//========= Copyright (c) Valve Corporation, All rights reserved. ============//
#ifndef C_EYEBALL_BOSS_H
#define C_EYEBALL_BOSS_H

#include "../../NextBot/C_NextBot.h"

#define EYEBALL_ANGRY	2
#define EYEBALL_GRUMPY	1
#define EYEBALL_CALM	0

//--------------------------------------------------------------------------------------------------------
/**
 * The client-side implementation of the Halloween Eyeball Boss
 */
class C_EyeballBoss : public C_NextBotCombatCharacter
{
public:
	M_NETVAR(m_lookAtSpot, Vector, "CEyeballBoss", "m_lookAtSpot");
	M_NETVAR(m_attitude, int, "CEyeballBoss", "m_attitude");
};

#endif // C_EYEBALL_BOSS_H