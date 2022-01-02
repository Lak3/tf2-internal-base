//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: CHud handles the message, calculation, and drawing the HUD
//
// $NoKeywords: $
//=============================================================================//
#ifndef HUD_H
#define HUD_H
#ifdef _WIN32
#pragma once
#endif

#include "../../public/tier1/bitbuf.h"
#include "../../public/tier1/utlvector.h"
#include "../../public/tier1/utldict.h"
#include "../../public/vgui/VGUI.h"
#include "../../public/Color.h"

namespace vgui
{
	class IScheme;
}

// basic rectangle struct used for drawing
typedef struct wrect_s
{
	int	left;
	int right;
	int top;
	int bottom;
} wrect_t;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CHudTexture
{
public:
	int Width() const
	{
		return rc.right - rc.left;
	}

	int Height() const
	{
		return rc.bottom - rc.top;
	}

	char		szShortName[64];
	char		szTextureFile[64];

	bool		bRenderUsingFont;
	bool		bPrecached;
	char		cCharacterInFont;
	vgui::HFont hFont;

	// vgui texture Id assigned to this item
	int			textureId;
	// s0, t0, s1, t1
	float		texCoords[4];

	// Original bounds
	wrect_t		rc;
};

class CHudElement;
class CHudRenderGroup;

//-----------------------------------------------------------------------------
// Purpose: Main hud manager
//-----------------------------------------------------------------------------
class CHud
{
public:
	//For progress bar orientations
	static const int			HUDPB_HORIZONTAL;
	static const int			HUDPB_VERTICAL;
	static const int			HUDPB_HORIZONTAL_INV;

public:
	void SetScreenShotTime(float flTime)
	{ 
		m_flScreenShotTime = flTime;
	}

	// Walk through all the HUD elements. Handler should be an object taking a CHudElement*
	template<typename THandler>
	void ForEachHudElement(THandler handler)
	{
		FOR_EACH_VEC(m_HudList, i)
		{
			handler(m_HudList[i]);
		}
	}

public:
	int							m_iKeyBits;
#ifndef _XBOX
	float						m_flMouseSensitivity;
	float						m_flMouseSensitivityFactor;
#endif
	float						m_flFOVSensitivityAdjust;

	Color						m_clrNormal;
	Color						m_clrCaution;
	Color						m_clrYellowish;

	CUtlVector<CHudElement*>	m_HudList;

	bool						m_bHudTexturesLoaded;

	// Global list of known icons
	CUtlDict<CHudTexture*, int>		m_Icons;

	CUtlVector<const char*>				m_RenderGroupNames;
	CUtlMap<int, CHudRenderGroup*>		m_RenderGroups;

	float						m_flScreenShotTime; // used to take end-game screenshots
};

#endif // HUD_H