//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef C_OBJ_SENTRYGUN_H
#define C_OBJ_SENTRYGUN_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseobject.h"

enum
{
	SHIELD_NONE = 0,
	SHIELD_NORMAL,	// 33% damage taken
	SHIELD_MAX		// 10% damage taken, no inactive period
};

//-----------------------------------------------------------------------------
// Purpose: Sentry object
//-----------------------------------------------------------------------------
class C_ObjectSentrygun : public C_BaseObject
{
public:
	M_NETVAR(m_iAmmoShells, int, "CObjectSentrygun", "m_iAmmoShells");
	M_NETVAR(m_iAmmoRockets, int, "CObjectSentrygun", "m_iAmmoRockets");
	M_NETVAR(m_iState, int, "CObjectSentrygun", "m_iState");
	M_NETVAR(m_bPlayerControlled, bool, "CObjectSentrygun", "m_bPlayerControlled");
	M_NETVAR(m_nShieldLevel, int, "CObjectSentrygun", "m_nShieldLevel");
	M_NETVAR(m_bShielded, bool, "CObjectSentrygun", "m_bShielded");
	M_NETVAR(m_hEnemy, EHANDLE, "CObjectSentrygun", "m_hEnemy");
	M_NETVAR(m_hAutoAimTarget, EHANDLE, "CObjectSentrygun", "m_hAutoAimTarget");
	M_NETVAR(m_iKills, int, "CObjectSentrygun", "m_iKills");
	M_NETVAR(m_iAssists, int, "CObjectSentrygun", "m_iAssists");
	
public:
	inline int m_iMaxAmmoShells()
	{
		if ((m_iUpgradeLevel() == 1) || m_bMiniBuilding())
			return SENTRYGUN_MAX_SHELLS_1;
		else
			return SENTRYGUN_MAX_SHELLS_3; //Both 2 and 3 are 200
	}

public:
	inline void GetAmmoCount(int& iShells, int& iMaxShells, int& iRockets, int& iMaxRockets)
	{
		const bool bIsMini = m_bMiniBuilding();

		iShells = m_iAmmoShells();
		iMaxShells = m_iMaxAmmoShells();
		iRockets = bIsMini ? 0 : m_iAmmoRockets();
		iMaxRockets = (bIsMini || (m_iUpgradeLevel() < 3)) ? 0 : SENTRYGUN_MAX_ROCKETS;
	}
};

#endif //C_OBJ_SENTRYGUN_H