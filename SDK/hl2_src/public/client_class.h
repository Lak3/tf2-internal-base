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
#if !defined( CLIENT_CLASS_H )
#define CLIENT_CLASS_H
#ifdef _WIN32
#pragma once
#endif

#include "tier1/interface.h"
#include "dt_recv.h"

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
class Vector;
class CMouthInfo;

//-----------------------------------------------------------------------------
// represents a handle used only by the client DLL
//-----------------------------------------------------------------------------
#include "iclientrenderable.h"
#include "iclientnetworkable.h"
#include "../game/shared/tf/tf_shareddefs.h"

class ClientClass;

// Linked list of all known client classes
extern ClientClass* g_pClientClassHead;

// The serial number that gets passed in is used for ehandles.
typedef IClientNetworkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

//-----------------------------------------------------------------------------
// Purpose: Client side class definition
//-----------------------------------------------------------------------------
class ClientClass
{
public:
	ClientClass(const char* pNetworkName, CreateClientClassFn createFn, CreateEventFn createEventFn, RecvTable* pRecvTable)
	{
		m_pNetworkName = pNetworkName;
		m_pCreateFn = createFn;
		m_pCreateEventFn = createEventFn;
		m_pRecvTable = pRecvTable;

		// Link it in
		m_pNext = g_pClientClassHead;
		g_pClientClassHead = this;
	}

	const char* GetName()
	{
		return m_pNetworkName;
	}

	ETFClientClass GetTFClientClass() const
	{
		return static_cast<ETFClientClass>(m_ClassID);
	}

public:
	CreateClientClassFn	m_pCreateFn;
	CreateEventFn		m_pCreateEventFn; //Only called for event objects.
	const char*         m_pNetworkName;
	RecvTable*          m_pRecvTable;
	ClientClass*        m_pNext;
	int					m_ClassID; //Managed by the engine.
};

#endif // CLIENT_CLASS_H