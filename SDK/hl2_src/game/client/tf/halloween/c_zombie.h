//========= Copyright (c) Valve Corporation, All rights reserved. ============//
#ifndef C_ZOMBIE_H
#define C_ZOMBIE_H

#include "../../NextBot/C_NextBot.h"

//--------------------------------------------------------------------------------------------------------
/**
 * The client-side implementation of the Halloween Zombie
 */
class C_Zombie : public C_NextBotCombatCharacter
{
public:
	M_NETVAR(m_flHeadScale, float, "CZombie", "m_flHeadScale");
};

#endif // C_EYEBALL_BOSS_H