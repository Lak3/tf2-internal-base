//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
// This is what all shaders inherit from.
//===========================================================================//
#ifndef BASESHADER_H
#define BASESHADER_H
#ifdef _WIN32
#pragma once
#endif

#include "../materialsystem/IShader.h"
#include "../materialsystem/imaterialvar.h"
#include "../materialsystem/ishaderapi.h"
#include "../materialsystem/imaterialsystemhardwareconfig.h"
#include "../tier1/convar.h"

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class IMaterialVar;

//-----------------------------------------------------------------------------
// Standard material vars
//-----------------------------------------------------------------------------
// Note: if you add to these, add to s_StandardParams in CBaseShader.cpp
enum ShaderMaterialVars_t
{
	FLAGS = 0,
	FLAGS_DEFINED,	// mask indicating if the flag was specified
	FLAGS2,
	FLAGS_DEFINED2,
	COLOR,
	ALPHA,
	BASETEXTURE,
	FRAME,
	BASETEXTURETRANSFORM,
	FLASHLIGHTTEXTURE,
	FLASHLIGHTTEXTUREFRAME,
	COLOR2,
	SRGBTINT,

	NUM_SHADER_MATERIAL_VARS
};

// Alpha belnd mode enums. Moved from basevsshader
enum BlendType_t
{
	// no alpha blending
	BT_NONE = 0,



	// src * srcAlpha + dst * (1-srcAlpha)
	// two passes for HDR:
	//		pass 1:
	//			color: src * srcAlpha + dst * (1-srcAlpha)
	//			alpha: srcAlpha * zero + dstAlpha * (1-srcAlpha)
	//		pass 2:
	//			color: none
	//			alpha: srcAlpha * one + dstAlpha * one
	//
	BT_BLEND,



	// src * one + dst * one
	// one pass for HDR
	BT_ADD,



	// Why do we ever use this instead of using premultiplied alpha?
	// src * srcAlpha + dst * one
	// two passes for HDR
	//		pass 1:
	//			color: src * srcAlpha + dst * one
	//			alpha: srcAlpha * one + dstAlpha * one
	//		pass 2:
	//			color: none
	//			alpha: srcAlpha * one + dstAlpha * one
	BT_BLENDADD
};

class CMeshBuilder;
typedef int SoftwareVertexShader_t;

//-----------------------------------------------------------------------------
// Base class for shaders, contains helper methods. INCOMPLETE
//-----------------------------------------------------------------------------
class CBaseShader : public IShader
{
public:
	// Methods inherited from IShader
	virtual char const* GetFallbackShader(IMaterialVar** params) const { return 0; }
	virtual int GetNumParams() const = 0;
	virtual char const* GetParamName(int paramIndex) const = 0;
	virtual char const* GetParamHelp(int paramIndex) const = 0;
	virtual ShaderParamType_t GetParamType(int paramIndex) const = 0;
	virtual char const* GetParamDefault(int paramIndex) const = 0;
	virtual int GetParamFlags(int nParamIndex) const = 0;

	virtual void InitShaderParams(IMaterialVar** ppParams, const char* pMaterialName) = 0;
	virtual void InitShaderInstance(IMaterialVar** ppParams, IShaderInit* pShaderInit, const char* pMaterialName, const char* pTextureGroupName) = 0;
	virtual void DrawElements(IMaterialVar** params, int nModulationFlags, IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI,
		VertexCompressionType_t vertexCompression, CBasePerMaterialContextData** pContext) = 0;

	virtual	const SoftwareVertexShader_t GetSoftwareVertexShader() const { return m_SoftwareVertexShader; }

public:
	// These functions must be implemented by the shader
	virtual void OnInitShaderParams(IMaterialVar** ppParams, const char* pMaterialName) {}
	virtual void OnInitShaderInstance(IMaterialVar** ppParams, IShaderInit* pShaderInit, const char* pMaterialName) = 0;
	virtual void OnDrawElements(IMaterialVar** params, IShaderShadow* pShaderShadow, IShaderDynamicAPI* pShaderAPI, VertexCompressionType_t vertexCompression, CBasePerMaterialContextData** pContextDataPtr) = 0;

	// Is the color var white?
	bool IsWhite(int colorVar);

	static IMaterialVar** s_ppParams;

protected:
	SoftwareVertexShader_t m_SoftwareVertexShader;

	static const char* s_pTextureGroupName; // Current material's texture group name.
	static IShaderShadow* s_pShaderShadow;
	static IShaderDynamicAPI* s_pShaderAPI;
	static IShaderInit* s_pShaderInit;

private:
	static int s_nModulationFlags;
	static CMeshBuilder* s_pMeshBuilder;
};

//-----------------------------------------------------------------------------
// Is the color var white?
//-----------------------------------------------------------------------------
inline bool CBaseShader::IsWhite(int colorVar)
{
	if (colorVar < 0)
		return true;

	if (!s_ppParams[colorVar]->IsDefined())
		return true;

	float color[3];
	s_ppParams[colorVar]->GetVecValue(color, 3);
	return (color[0] >= 1.0f) && (color[1] >= 1.0f) && (color[2] >= 1.0f);
}


class CBasePerMaterialContextData								// shaders can keep per material data in classes descended from this
{
public:
	uint32 m_nVarChangeID;
	bool m_bMaterialVarsChanged;							// set by mat system when material vars change. shader should rehtink and then clear the var

	FORCEINLINE CBasePerMaterialContextData(void)
	{
		m_bMaterialVarsChanged = true;
		m_nVarChangeID = 0xffffffff;
	}

	// virtual destructor so that derived classes can have their own data to be cleaned up on
	// delete of material
	virtual ~CBasePerMaterialContextData(void)
	{
	}
};

#endif // BASESHADER_H