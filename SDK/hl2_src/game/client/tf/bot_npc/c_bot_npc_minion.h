//========= Copyright (c) Valve Corporation, All rights reserved. ============//
#ifndef C_BOT_NPC_MINION_H
#define C_BOT_NPC_MINION_H

#include "../../NextBot/C_NextBot.h"

//--------------------------------------------------------------------------------------------------------
/**
 * The client-side implementation of Bot NPC Minion
 */
class C_BotNPCMinion : public C_NextBotCombatCharacter
{
public:
	M_NETVAR(m_stunTarget, int, "CBotNPCMinion", "m_stunTarget");
};

#endif // C_BOT_NPC_MINION_H