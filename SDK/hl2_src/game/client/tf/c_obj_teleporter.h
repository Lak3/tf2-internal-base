//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef C_OBJ_TELEPORTER_H
#define C_OBJ_TELEPORTER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseobject.h"

class C_ObjectTeleporter : public C_BaseObject
{
public:
	M_NETVAR(m_iState, int, "CObjectTeleporter", "m_iState");
	M_NETVAR(m_flRechargeTime, float, "CObjectTeleporter", "m_flRechargeTime");
	M_NETVAR(m_flCurrentRechargeDuration, float, "CObjectTeleporter", "m_flCurrentRechargeDuration");
	M_NETVAR(m_iTimesUsed, int, "CObjectTeleporter", "m_iTimesUsed");
	M_NETVAR(m_flYawToExit, float, "CObjectTeleporter", "m_flYawToExit");
	M_NETVAR(m_bMatchBuilding, bool, "CObjectTeleporter", "m_bMatchBuilding");

public:
	float GetChargeTime(void)
	{
		const float flTime = (m_flRechargeTime() - I::GlobalVars->curtime);

		if (flTime < 0.0f)
			return 0.0f;

		return flTime;
	}
};

#endif //C_OBJ_TELEPORTER_H