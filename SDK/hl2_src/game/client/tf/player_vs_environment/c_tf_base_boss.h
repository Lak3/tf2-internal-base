//========= Copyright (c) Valve Corporation, All rights reserved. ============//
#ifndef C_TF_BASE_BOSS_H
#define C_TF_BASE_BOSS_H

#include "../../NextBot/C_NextBot.h"
#include "../../tf/c_tf_mvm_boss_progress_user.h"

class C_TFBaseBoss : public C_NextBotCombatCharacter, public C_TFMvMBossProgressUser
{
public:
	M_NETVAR(m_lastHealthPercentage, float, "CTFBaseBoss", "m_lastHealthPercentage");
};

#endif // C_TF_BASE_BOSS_H