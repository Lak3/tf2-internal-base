//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef C_OBJ_DISPENSER_H
#define C_OBJ_DISPENSER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseobject.h"

class C_ObjectDispenser : public C_BaseObject
{
public:
	M_NETVAR(m_iState, int, "CObjectDispenser", "m_iState");
	M_NETVAR(m_iAmmoMetal, int, "CObjectDispenser", "m_iAmmoMetal");
	M_NETVAR(m_iMiniBombCounter, int, "CObjectDispenser", "m_iMiniBombCounter");

public:
	inline int GetMaxMetal() {
		return m_bMiniBuilding() ? MINI_DISPENSER_MAX_METAL : DISPENSER_MAX_METAL_AMMO;
	}
};

#endif //C_OBJ_DISPENSER_H