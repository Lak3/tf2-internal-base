//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef IVEHICLE_H
#define IVEHICLE_H
#ifdef _WIN32
#pragma once
#endif

#include "../client/c_baseplayer.h"

class CUserCmd;
class IMoveHelper;
class CMoveData;
class C_BaseCombatCharacter;

// This is used by the player to access vehicles. It's an interface so the
// vehicles are not restricted in what they can derive from.
abstract_class IVehicle
{
public:
	// Get and set the current driver. Use PassengerRole_t enum in shareddefs.h for adding passengers
	virtual C_BaseCombatCharacter* GetPassenger(int nRole = VEHICLE_ROLE_DRIVER) = 0;
	virtual int					  GetPassengerRole(C_BaseCombatCharacter* pPassenger) = 0;

	// Where is the passenger seeing from?
	virtual void			GetVehicleViewPosition(int nRole, Vector* pOrigin, QAngle* pAngles, float* pFOV = NULL) = 0;

	// Does the player use his normal weapons while in this mode?
	virtual bool			IsPassengerUsingStandardWeapons(int nRole = VEHICLE_ROLE_DRIVER) = 0;

	// Process movement
	virtual void			SetupMove(C_BasePlayer* player, CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move) = 0;
	virtual void			ProcessMovement(C_BasePlayer* pPlayer, CMoveData* pMoveData) = 0;
	virtual void			FinishMove(C_BasePlayer* player, CUserCmd* ucmd, CMoveData* move) = 0;

	// Process input
	virtual void			ItemPostFrame(C_BasePlayer* pPlayer) = 0;
};

#endif // IVEHICLE_H