//========= Copyright (c) Valve Corporation, All rights reserved. ============//
#ifndef C_BOT_NPC_H
#define C_BOT_NPC_H

#include "../../NextBot/C_NextBot.h"

//--------------------------------------------------------------------------------------------------------
/**
 * The client-side implementation of Bot NPC
 */
class C_BotNPC : public C_NextBotCombatCharacter
{
public:
	M_NETVAR(m_laserTarget, int, "CBotNPC", "m_laserTarget");
	M_NETVAR(m_isNuking, int, "CBotNPC", "m_isNuking");
};

#endif // C_BOT_NPC_H