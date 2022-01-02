//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
//
//
//=============================================================================
#ifndef C_MERASMUS_H
#define C_MERASMUS_H

#include "../../NextBot/C_NextBot.h"

//--------------------------------------------------------------------------------------------------------
/**
 * The client-side implementation of the Dark Knight
 */
class C_Merasmus : public C_NextBotCombatCharacter
{
public:
	M_NETVAR(m_bRevealed, bool, "CMerasmus", "m_bRevealed");
	M_NETVAR(m_bIsDoingAOEAttack, bool, "CMerasmus", "m_bIsDoingAOEAttack");
	M_NETVAR(m_bStunned, bool, "CMerasmus", "m_bStunned");
};

#endif // C_MERASMUS_H