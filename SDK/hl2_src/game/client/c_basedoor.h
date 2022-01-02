//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#if !defined( C_BASEDOOR_H )
#define C_BASEDOOR_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseentity.h"

class C_BaseDoor : public C_BaseEntity
{
public:
	virtual ~C_BaseDoor() = 0;

public:
	M_NETVAR(m_flWaveHeight, float, "CBaseDoor", "m_flWaveHeight");
};

#endif // C_BASEDOOR_H