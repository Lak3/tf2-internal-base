//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef C_BASEANIMATINGOVERLAY_H
#define C_BASEANIMATINGOVERLAY_H
#pragma once

#include "c_baseanimating.h"

class C_BaseAnimatingOverlay : public C_BaseAnimating
{
public:
	virtual ~C_BaseAnimatingOverlay() = 0;

public:
	M_NETVAR(m_flPrevCycle, float, "CBaseAnimatingOverlay", "m_flPrevCycle");
	M_NETVAR(m_flWeight, float, "CBaseAnimatingOverlay", "m_flWeight");
	M_NETVAR(m_nOrder, int, "CBaseAnimatingOverlay", "m_nOrder");
};

#endif //C_BASEANIMATINGOVERLAY_H