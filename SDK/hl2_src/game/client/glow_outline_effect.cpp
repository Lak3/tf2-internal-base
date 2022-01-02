//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Functionality to render a glowing outline around client renderable objects.
//
//===============================================================================
#include "glow_outline_effect.h"
#include "../../public/model_types.h"
#include "../../public/materialsystem/imaterialvar.h"
#include "../../public/materialsystem/itexture.h"
#include "../../public/view_shared.h"
#include "../../public/ivrenderview.h"

#define FULL_FRAME_TEXTURE "_rt_FullFrameFB"

bool glow_outline_effect_enable = false;
float glow_outline_effect_width = 1.0f;

CGlowObjectManager g_GlowObjectManager;

void CGlowObjectManager::RenderGlowEffects(const CViewSetup* pSetup, int nSplitScreenSlot)
{
	if (I::MaterialSystemHardwareConfig->SupportsPixelShaders_2_0())
	{
		if (glow_outline_effect_enable)
		{
			CMatRenderContextPtr pRenderContext(I::MaterialSystem);

			int nX, nY, nWidth, nHeight;
			pRenderContext->GetViewport(nX, nY, nWidth, nHeight);

			PIXEvent _pixEvent(pRenderContext, "EntityGlowEffects");
			ApplyEntityGlowEffects(pSetup, nSplitScreenSlot, pRenderContext, glow_outline_effect_width, nX, nY, nWidth, nHeight);
		}
	}
}

static void SetRenderTargetAndViewPort(ITexture* rt, int w, int h)
{
	CMatRenderContextPtr pRenderContext(I::MaterialSystem);
	pRenderContext->SetRenderTarget(rt);
	pRenderContext->Viewport(0, 0, w, h);
}

void CGlowObjectManager::RenderGlowModels(const CViewSetup* pSetup, int nSplitScreenSlot, CMatRenderContextPtr& pRenderContext)
{
	//==========================================================================================//
	// This renders solid pixels with the correct coloring for each object that needs the glow.	//
	// After this function returns, this image will then be blurred and added into the frame	//
	// buffer with the objects stenciled out.													//
	//==========================================================================================//
	pRenderContext->PushRenderTargetAndViewport();

	// Save modulation color and blend
	Vector vOrigColor;
	I::RenderView->GetColorModulation(vOrigColor.Base());
	float flOrigBlend = I::RenderView->GetBlend();

	// Get pointer to FullFrameFB
	ITexture* pRtFullFrame = NULL;
	pRtFullFrame = I::MaterialSystem->FindTexture(FULL_FRAME_TEXTURE, TEXTURE_GROUP_RENDER_TARGET);

	SetRenderTargetAndViewPort(pRtFullFrame, pSetup->width, pSetup->height);

	pRenderContext->ClearColor3ub(0, 0, 0);
	pRenderContext->ClearBuffers(true, false, false);

	// Set override material for glow color
	IMaterial* pMatGlowColor = NULL;

	pMatGlowColor = I::MaterialSystem->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER, true);
	I::StudioRender->ForcedMaterialOverride(pMatGlowColor);

	ShaderStencilState_t stencilState;
	stencilState.m_bEnable = false;
	stencilState.m_nReferenceValue = 0;
	stencilState.m_nTestMask = 0xFF;
	stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
	stencilState.m_PassOp = STENCILOPERATION_KEEP;
	stencilState.m_FailOp = STENCILOPERATION_KEEP;
	stencilState.m_ZFailOp = STENCILOPERATION_KEEP;

	stencilState.SetStencilState(pRenderContext);

	//==================//
	// Draw the objects //
	//==================//
	for (int i = 0; i < m_GlowObjectDefinitions.Count(); ++i)
	{
		if (m_GlowObjectDefinitions[i].IsUnused() || !m_GlowObjectDefinitions[i].ShouldDraw(nSplitScreenSlot))
			continue;

		I::RenderView->SetBlend(m_GlowObjectDefinitions[i].m_flGlowAlpha);
		Vector vGlowColor = m_GlowObjectDefinitions[i].m_vGlowColor * m_GlowObjectDefinitions[i].m_flGlowAlpha;
		I::RenderView->SetColorModulation(&vGlowColor[0]); // This only sets rgb, not alpha

		m_GlowObjectDefinitions[i].DrawModel();
	}

	//if (g_bDumpRenderTargets)
	//{
	//	DumpTGAofRenderTarget(pSetup->width, pSetup->height, "GlowModels");
	//}

	I::StudioRender->ForcedMaterialOverride(NULL);
	I::RenderView->SetColorModulation(vOrigColor.Base());
	I::RenderView->SetBlend(flOrigBlend);

	ShaderStencilState_t stencilStateDisable;
	stencilStateDisable.m_bEnable = false;
	stencilStateDisable.SetStencilState(pRenderContext);

	pRenderContext->PopRenderTargetAndViewport();
}

void CGlowObjectManager::ApplyEntityGlowEffects(const CViewSetup* pSetup, int nSplitScreenSlot, CMatRenderContextPtr& pRenderContext, float flBloomScale, int x, int y, int w, int h)
{
	//=======================================================//
	// Render objects into stencil buffer					 //
	//=======================================================//
	// Set override shader to the same simple shader we use to render the glow models
	IMaterial* pMatGlowColor = I::MaterialSystem->FindMaterial("dev/glow_color", TEXTURE_GROUP_OTHER, true);
	I::StudioRender->ForcedMaterialOverride(pMatGlowColor);

	ShaderStencilState_t stencilStateDisable;
	stencilStateDisable.m_bEnable = false;
	float flSavedBlend = I::RenderView->GetBlend();

	// Set alpha to 0 so we don't touch any color pixels
	I::RenderView->SetBlend(0.0f);
	pRenderContext->OverrideDepthEnable(true, false);

	int iNumGlowObjects = 0;

	for (int i = 0; i < m_GlowObjectDefinitions.Count(); ++i)
	{
		if (m_GlowObjectDefinitions[i].IsUnused() || !m_GlowObjectDefinitions[i].ShouldDraw(nSplitScreenSlot))
			continue;

		if (m_GlowObjectDefinitions[i].m_bRenderWhenOccluded || m_GlowObjectDefinitions[i].m_bRenderWhenUnoccluded)
		{
			if (m_GlowObjectDefinitions[i].m_bRenderWhenOccluded && m_GlowObjectDefinitions[i].m_bRenderWhenUnoccluded)
			{
				ShaderStencilState_t stencilState;
				stencilState.m_bEnable = true;
				stencilState.m_nReferenceValue = 1;
				stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
				stencilState.m_PassOp = STENCILOPERATION_REPLACE;
				stencilState.m_FailOp = STENCILOPERATION_KEEP;
				stencilState.m_ZFailOp = STENCILOPERATION_REPLACE;

				stencilState.SetStencilState(pRenderContext);

				m_GlowObjectDefinitions[i].DrawModel();
			}
			else if (m_GlowObjectDefinitions[i].m_bRenderWhenOccluded)
			{
				ShaderStencilState_t stencilState;
				stencilState.m_bEnable = true;
				stencilState.m_nReferenceValue = 1;
				stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
				stencilState.m_PassOp = STENCILOPERATION_KEEP;
				stencilState.m_FailOp = STENCILOPERATION_KEEP;
				stencilState.m_ZFailOp = STENCILOPERATION_REPLACE;

				stencilState.SetStencilState(pRenderContext);

				m_GlowObjectDefinitions[i].DrawModel();
			}
			else if (m_GlowObjectDefinitions[i].m_bRenderWhenUnoccluded)
			{
				ShaderStencilState_t stencilState;
				stencilState.m_bEnable = true;
				stencilState.m_nReferenceValue = 2;
				stencilState.m_nTestMask = 0x1;
				stencilState.m_nWriteMask = 0x3;
				stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_EQUAL;
				stencilState.m_PassOp = STENCILOPERATION_INCRSAT;
				stencilState.m_FailOp = STENCILOPERATION_KEEP;
				stencilState.m_ZFailOp = STENCILOPERATION_REPLACE;

				stencilState.SetStencilState(pRenderContext);

				m_GlowObjectDefinitions[i].DrawModel();
			}
		}

		iNumGlowObjects++;
	}

	// Need to do a 2nd pass to warm stencil for objects which are rendered only when occluded
	for (int i = 0; i < m_GlowObjectDefinitions.Count(); ++i)
	{
		if (m_GlowObjectDefinitions[i].IsUnused() || !m_GlowObjectDefinitions[i].ShouldDraw(nSplitScreenSlot))
			continue;

		if (m_GlowObjectDefinitions[i].m_bRenderWhenOccluded && !m_GlowObjectDefinitions[i].m_bRenderWhenUnoccluded)
		{
			ShaderStencilState_t stencilState;
			stencilState.m_bEnable = true;
			stencilState.m_nReferenceValue = 2;
			stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
			stencilState.m_PassOp = STENCILOPERATION_REPLACE;
			stencilState.m_FailOp = STENCILOPERATION_KEEP;
			stencilState.m_ZFailOp = STENCILOPERATION_KEEP;
			stencilState.SetStencilState(pRenderContext);

			m_GlowObjectDefinitions[i].DrawModel();
		}
	}

	pRenderContext->OverrideDepthEnable(false, false);
	I::RenderView->SetBlend(flSavedBlend);
	stencilStateDisable.SetStencilState(pRenderContext);
	I::StudioRender->ForcedMaterialOverride(NULL);

	// If there aren't any objects to glow, don't do all this other stuff
	// this fixes a bug where if there are glow objects in the list, but none of them are glowing,
	// the whole screen blooms.
	if (iNumGlowObjects <= 0)
		return;

	//=============================================
	// Render the glow colors to _rt_FullFrameFB 
	//=============================================
	{
		PIXEvent pixEvent(pRenderContext, "RenderGlowModels");
		RenderGlowModels(pSetup, nSplitScreenSlot, pRenderContext);
	}

	// Get viewport
	int nSrcWidth = pSetup->width;
	int nSrcHeight = pSetup->height;
	int nViewportX, nViewportY, nViewportWidth, nViewportHeight;
	pRenderContext->GetViewport(nViewportX, nViewportY, nViewportWidth, nViewportHeight);

	// Get material and texture pointers
	ITexture* pRtQuarterSize1 = I::MaterialSystem->FindTexture("_rt_SmallFB1", TEXTURE_GROUP_RENDER_TARGET);

	{
		//=======================================================================================================//
		// At this point, pRtQuarterSize0 is filled with the fully colored glow around everything as solid glowy //
		// blobs. Now we need to stencil out the original objects by only writing pixels that have no            //
		// stencil bits set in the range we care about.                                                          //
		//=======================================================================================================//
		IMaterial* pMatHaloAddToScreen = I::MaterialSystem->FindMaterial("dev/halo_add_to_screen", TEXTURE_GROUP_OTHER, true);

		// Do not fade the glows out at all (weight = 1.0)
		IMaterialVar* pDimVar = pMatHaloAddToScreen->FindVar("$C0_X", NULL);
		pDimVar->SetFloatValue(1.0f);

		// Set stencil state
		ShaderStencilState_t stencilState;
		stencilState.m_bEnable = true;
		stencilState.m_nWriteMask = 0x0; // We're not changing stencil
		stencilState.m_nTestMask = 0xFF;
		stencilState.m_nReferenceValue = 0x0;
		stencilState.m_CompareFunc = STENCILCOMPARISONFUNCTION_EQUAL;
		stencilState.m_PassOp = STENCILOPERATION_KEEP;
		stencilState.m_FailOp = STENCILOPERATION_KEEP;
		stencilState.m_ZFailOp = STENCILOPERATION_KEEP;
		stencilState.SetStencilState(pRenderContext);

		// Draw quad
		pRenderContext->DrawScreenSpaceRectangle(pMatHaloAddToScreen, 0, 0, nViewportWidth, nViewportHeight,
			0.0f, -0.5f, nSrcWidth / 4 - 1, nSrcHeight / 4 - 1,
			pRtQuarterSize1->GetActualWidth(),
			pRtQuarterSize1->GetActualHeight());

		stencilStateDisable.SetStencilState(pRenderContext);
	}
}

void CGlowObjectManager::GlowObjectDefinition_t::DrawModel()
{
	if (m_hEntity.Get())
	{
		m_hEntity->DrawModel(STUDIO_RENDER);
		C_BaseEntity* pAttachment = m_hEntity->FirstMoveChild();

		while (pAttachment != NULL)
		{
			if (!g_GlowObjectManager.HasGlowEffect(pAttachment) && pAttachment->ShouldDraw())
			{
				pAttachment->DrawModel(STUDIO_RENDER);
			}

			pAttachment = pAttachment->NextMovePeer();
		}
	}
}