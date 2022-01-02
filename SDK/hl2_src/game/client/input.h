//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//===========================================================================//
#if !defined( INPUT_H )
#define INPUT_H
#ifdef _WIN32
#pragma once
#endif

#include "iinput.h"
#include "kbutton.h"
#include "../shared/ehandle.h"
#include "../shared/usercmd.h"
#include "../../public/mathlib/vector.h"
#include "../../public/inputsystem/AnalogCode.h"

typedef unsigned int CRC32_t;

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CKeyboardKey
{
public:
	// Name for key
	char				name[32];
	// Pointer to the underlying structure
	kbutton_t* pkey;
	// Next key in key list.
	CKeyboardKey* next;
};

class ConVar;

class CInput : public IInput
{
// Interface
public:
	virtual float CAM_CapYaw(float fVal) const { return fVal; }
	virtual float CAM_CapPitch(float fVal) const { return fVal; }

	virtual float JoyStickAdjustYaw(float flSpeed) { return flSpeed; }

// Public Data
public:
	typedef struct
	{
		unsigned int AxisFlags;
		unsigned int AxisMap;
		unsigned int ControlMap;
	} joy_axis_t;

	enum
	{
		GAME_AXIS_NONE = 0,
		GAME_AXIS_FORWARD,
		GAME_AXIS_PITCH,
		GAME_AXIS_SIDE,
		GAME_AXIS_YAW,
		MAX_GAME_AXES
	};

	enum
	{
		CAM_COMMAND_NONE = 0,
		CAM_COMMAND_TOTHIRDPERSON = 1,
		CAM_COMMAND_TOFIRSTPERSON = 2
	};

	enum
	{
		MOUSE_ACCEL_THRESHHOLD1 = 0,	// if mouse moves > this many mickey's double it
		MOUSE_ACCEL_THRESHHOLD2,		// if mouse moves > this many mickey's double it a second time
		MOUSE_SPEED_FACTOR,				// 0 = disabled, 1 = threshold 1 enabled, 2 = threshold 2 enabled

		NUM_MOUSE_PARAMS,
	};

	class CVerifiedUserCmd
	{
	public:
		CUserCmd	m_cmd;
		CRC32_t		m_crc;
	};

	inline CUserCmd* m_pCommands() {
		return *reinterpret_cast<CUserCmd**>(reinterpret_cast<DWORD>(this) + 0xFC);
	}

	inline CVerifiedUserCmd* m_pVerifiedCommands() {
		return *reinterpret_cast<CVerifiedUserCmd**>(reinterpret_cast<DWORD>(this) + 0x100);
	}
};

namespace I { inline CInput* Input = nullptr; }

#endif // INPUT_H