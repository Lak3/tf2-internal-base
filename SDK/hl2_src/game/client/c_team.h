//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Client side CTeam class
//
// $NoKeywords: $
//=============================================================================//
#ifndef C_TEAM_H
#define C_TEAM_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseentity.h"

class C_BasePlayer;

class C_Team : public C_BaseEntity
{
public:
	virtual			~C_Team() = 0;

	virtual char*	Get_Name(void) = 0;
	virtual int		Get_Score(void) = 0;
	virtual int		Get_Deaths(void) = 0;
	virtual int		Get_Ping(void) = 0;
	virtual int		Get_Number_Players(void) = 0;
	virtual bool	ContainsPlayer(int iPlayerIndex) = 0;
	virtual int		GetNumPlayers(void) = 0;

public:
	M_NETVAR(m_iTeamNum, int, "CTeam", "m_iTeamNum");
	M_NETVAR(m_iScore, int, "CTeam", "m_iScore");
	M_NETVAR(m_iRoundsWon, int, "CTeam", "m_iRoundsWon");
	M_NETVAR(m_szTeamname, const char*, "CTeam", "m_szTeamname");
};

#endif // C_TEAM_H