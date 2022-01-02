//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $NoKeywords: $
//===========================================================================//
#include "MaterialSystemUtil.h"
#include "imaterial.h"
#include "itexture.h"
#include "imaterialsystem.h"

//-----------------------------------------------------------------------------
// Little utility class to deal with material references
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------
CMaterialReference::CMaterialReference(char const* pMaterialName, const char* pTextureGroupName, bool bComplain) : m_pMaterial(0)
{
	if (pMaterialName)
	{
		assert(pTextureGroupName);
		Init(pMaterialName, pTextureGroupName, bComplain);
	}
}

CMaterialReference::~CMaterialReference()
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Attach to a material
//-----------------------------------------------------------------------------
void CMaterialReference::Init(char const* pMaterialName, const char* pTextureGroupName, bool bComplain)
{
	IMaterial* pMaterial = I::MaterialSystem->FindMaterial(pMaterialName, pTextureGroupName, bComplain);
	
	if (IsErrorMaterial(pMaterial))
	{
		if (IsOSX())
		{
			printf("\n ##### CMaterialReference::Init got error material for %s in tex group %s", pMaterialName, pTextureGroupName);
		}
	}

	assert(pMaterial);
	Init(pMaterial);
}

void CMaterialReference::Init(const char* pMaterialName, KeyValues* pVMTKeyValues)
{
	// CreateMaterial has a refcount of 1
	Shutdown();
	m_pMaterial = I::MaterialSystem->CreateMaterial(pMaterialName, pVMTKeyValues);
}

void CMaterialReference::Init(const char* pMaterialName, const char* pTextureGroupName, KeyValues* pVMTKeyValues)
{
	IMaterial* pMaterial = I::MaterialSystem->FindProceduralMaterial(pMaterialName, pTextureGroupName, pVMTKeyValues);
	assert(pMaterial);
	Init(pMaterial);
}

void CMaterialReference::Init(IMaterial* pMaterial)
{
	if (m_pMaterial != pMaterial)
	{
		Shutdown();
		m_pMaterial = pMaterial;
		if (m_pMaterial)
		{
			m_pMaterial->IncrementReferenceCount();
		}
	}
}

void CMaterialReference::Init(CMaterialReference& ref)
{
	if (m_pMaterial != ref.m_pMaterial)
	{
		Shutdown();
		m_pMaterial = ref.m_pMaterial;
		if (m_pMaterial)
		{
			m_pMaterial->IncrementReferenceCount();
		}
	}
}

//-----------------------------------------------------------------------------
// Detach from a material
//-----------------------------------------------------------------------------
void CMaterialReference::Shutdown()
{
	if (m_pMaterial && I::MaterialSystem)
	{
		m_pMaterial->DecrementReferenceCount();
		m_pMaterial = NULL;
	}
}

//-----------------------------------------------------------------------------
// Little utility class to deal with texture references
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------
CTextureReference::CTextureReference() : m_pTexture(NULL)
{
}

CTextureReference::CTextureReference(const CTextureReference& ref)
{
	m_pTexture = ref.m_pTexture;
	if (m_pTexture)
	{
		m_pTexture->IncrementReferenceCount();
	}
}

void CTextureReference::operator=(CTextureReference& ref)
{
	m_pTexture = ref.m_pTexture;
	if (m_pTexture)
	{
		m_pTexture->IncrementReferenceCount();
	}
}

CTextureReference::~CTextureReference()
{
	Shutdown();
}

//-----------------------------------------------------------------------------
// Attach to a texture
//-----------------------------------------------------------------------------
void CTextureReference::Init(char const* pTextureName, const char* pTextureGroupName, bool bComplain)
{
	Shutdown();
	m_pTexture = I::MaterialSystem->FindTexture(pTextureName, pTextureGroupName, bComplain);
	if (m_pTexture)
	{
		m_pTexture->IncrementReferenceCount();
	}
}

void CTextureReference::Init(ITexture* pTexture)
{
	Shutdown();

	m_pTexture = pTexture;
	if (m_pTexture)
	{
		m_pTexture->IncrementReferenceCount();
	}
}

void CTextureReference::InitProceduralTexture(const char* pTextureName, const char* pTextureGroupName, int w, int h, ImageFormat fmt, int nFlags)
{
	Shutdown();

	m_pTexture = I::MaterialSystem->CreateProceduralTexture(pTextureName, pTextureGroupName, w, h, fmt, nFlags);

	// NOTE: The texture reference is already incremented internally above!
	/*
	if ( m_pTexture )
	{
		m_pTexture->IncrementReferenceCount();
	}
	*/
}

void CTextureReference::InitRenderTarget(int w, int h, RenderTargetSizeMode_t sizeMode, ImageFormat fmt, MaterialRenderTargetDepth_t depth, bool bHDR, char* pStrOptionalName /* = NULL */)
{
	Shutdown();

	int textureFlags = TEXTUREFLAGS_CLAMPS | TEXTUREFLAGS_CLAMPT;
	if (depth == MATERIAL_RT_DEPTH_ONLY)
		textureFlags |= TEXTUREFLAGS_POINTSAMPLE;

	int renderTargetFlags = bHDR ? CREATERENDERTARGETFLAGS_HDR : 0;

	// NOTE: Refcount returned by CreateRenderTargetTexture is 1
	m_pTexture = I::MaterialSystem->CreateNamedRenderTargetTextureEx(pStrOptionalName, w, h, sizeMode, fmt,
		depth, textureFlags, renderTargetFlags);

	assert(m_pTexture);
}

//-----------------------------------------------------------------------------
// Detach from a texture
//-----------------------------------------------------------------------------
void CTextureReference::Shutdown(bool bDeleteIfUnReferenced)
{
	if (m_pTexture && I::MaterialSystem)
	{
		m_pTexture->DecrementReferenceCount();
		if (bDeleteIfUnReferenced)
		{
			m_pTexture->DeleteIfUnreferenced();
		}
		m_pTexture = NULL;
	}
}