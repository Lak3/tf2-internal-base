//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: A class embedded in players to provide hints to that player
//
//=============================================================================
#ifndef HINTSYSTEM_H
#define HINTSYSTEM_H
#ifdef _WIN32
#pragma once
#endif

#include "../client/c_baseplayer.h"

typedef bool (*HintTimerCallback)(C_BasePlayer* pOnPlayer);

// Derive from this if you have an entity that wants to display a hint
// when the player waves his target ID over it on the client.
abstract_class ITargetIDProvidesHint
{
public:
	virtual void DisplayHintTo(C_BasePlayer* pPlayer) = 0;
};

#endif // HINTSYSTEM_H