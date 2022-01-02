//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef C_AI_BASENPC_H
#define C_AI_BASENPC_H
#ifdef _WIN32
#pragma once
#endif

#include "c_basecombatcharacter.h"

// NOTE: Moved all controller code into c_basestudiomodel
class C_AI_BaseNPC : public C_BaseCombatCharacter
{
public:
	M_NETVAR(m_lifeState, unsigned int, "CAI_BaseNPC", "m_lifeState");
	M_NETVAR(m_bPerformAvoidance, bool, "CAI_BaseNPC", "m_bPerformAvoidance");
	M_NETVAR(m_bIsMoving, bool, "CAI_BaseNPC", "m_bIsMoving");
	M_NETVAR(m_bFadeCorpse, bool, "CAI_BaseNPC", "m_bFadeCorpse");
	M_NETVAR(m_iDeathPose, int, "CAI_BaseNPC", "m_iDeathPose");
	M_NETVAR(m_iDeathFrame, int, "CAI_BaseNPC", "m_iDeathFrame");
	M_NETVAR(m_iSpeedModRadius, int, "CAI_BaseNPC", "m_iSpeedModRadius");
	M_NETVAR(m_iSpeedModSpeed, int, "CAI_BaseNPC", "m_iSpeedModSpeed");
	M_NETVAR(m_bSpeedModActive, bool, "CAI_BaseNPC", "m_bSpeedModActive");
	M_NETVAR(m_bImportanRagdoll, bool, "CAI_BaseNPC", "m_bImportanRagdoll");
	M_NETVAR(m_flTimePingEffect, float, "CAI_BaseNPC", "m_flTimePingEffect");
};

#endif // C_AI_BASENPC_H