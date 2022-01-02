//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//
#if !defined( CLIENT_H )
#define CLIENT_H
#ifdef _WIN32
#pragma once
#endif

#include "../public/tier0/platform.h"
#include "../public/inetchannel.h"

struct model_t;
struct SoundInfo_t;

class ClientClass;
class CSfxTable;
class CPureServerWhitelist;

#define	MAX_DEMOS		32

struct AddAngle
{
	float total;
	float starttime;
};

class CClockDriftMgr_pad
{
public:
	enum { NUM_CLOCKDRIFT_SAMPLES = 16 };
	float m_ClockOffsets[NUM_CLOCKDRIFT_SAMPLES];
	int m_iCurClockOffset;
	int m_nServerTick;
	int	m_nClientTick;
};

//-----------------------------------------------------------------------------
// Purpose: CClientState should hold all pieces of the client state
//   The client_state_t structure is wiped completely at every server signon
//-----------------------------------------------------------------------------
class CClientState //: public CBaseClientState, public CClientFrameManager
{
public:
	byte pad0[0x10];
	INetChannel* m_NetChannel;
	byte pad1[0x140];
	CClockDriftMgr_pad m_ClockDriftMgr;
	int m_nDeltaTick;
	byte pad2[0x110];
	int m_nMaxClients;
	byte pad3[0x486C];
	int lastoutgoingcommand;
	int chokedcommands;
	int last_command_ack;
};

namespace I { inline CClientState* ClientState = nullptr; }

#endif // CLIENT_H