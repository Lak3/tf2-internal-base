//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H
#ifdef _WIN32
#pragma once
#endif

#include "string_t.h"
#include "mathlib/vector.h"

class CPlayerState
{
public:
	// This virtual method is necessary to generate a vtable in all cases
	// (DECLARE_PREDICTABLE will generate a vtable also)!
	virtual ~CPlayerState() {}

	// true if the player is dead
	bool        deadflag;
	// Viewing angle (player only)
	QAngle		v_angle;
};

#endif // PLAYERSTATE_H