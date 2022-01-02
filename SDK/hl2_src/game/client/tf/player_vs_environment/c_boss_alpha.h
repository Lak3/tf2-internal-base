//========= Copyright (c) Valve Corporation, All rights reserved. ============//
#ifndef C_BOSS_ALPHA_H
#define C_BOSS_ALPHA_H

#include "../../NextBot/C_NextBot.h"

//--------------------------------------------------------------------------------------------------------
/**
 * The client-side implementation of Boss Alpha
 */
class C_BossAlpha : public C_NextBotCombatCharacter
{
public:
	M_NETVAR(m_isNuking, int, "CBossAlpha", "m_isNuking");
};

#endif // C_BOSS_ALPHA_H