//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
//
//
//=============================================================================
#ifndef TF_BOT_HINT_ENGINEER_NEST_H
#define TF_BOT_HINT_ENGINEER_NEST_H

#include "../../../c_baseentity.h"

class C_TFBotHintEngineerNest : public C_BaseEntity
{
public:
	M_NETVAR(m_bHasActiveTeleporter, bool, "CTFBotHintEngineerNest", "m_bHasActiveTeleporter");
};

#endif // TF_BOT_HINT_ENGINEER_NEST_H