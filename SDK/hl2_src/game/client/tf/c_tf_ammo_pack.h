//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#ifndef C_TF_AMMO_PACK_H
#define C_TF_AMMO_PACK_H
#ifdef _WIN32
#pragma once
#endif

#include "../c_baseanimating.h"
#include "../../shared/hintsystem.h"

class C_TFAmmoPack : public C_BaseAnimating, public ITargetIDProvidesHint
{
public:
	virtual		~C_TFAmmoPack() = 0;
	virtual int GetWorldModelIndex(void) = 0;

public:
	M_NETVAR(m_vecInitialVelocity, Vector, "CTFAmmoPack", "m_vecInitialVelocity");
	M_NETVAR(m_angRotation, Vector, "CTFAmmoPack", "m_angRotation[0]");
};

#endif //C_TF_AMMO_PACK_H