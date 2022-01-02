//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef GAME_EVENT_LISTENER_H
#define GAME_EVENT_LISTENER_H
#ifdef _WIN32
#pragma once
#endif

#include "../../public/igameevents.h"

// A safer method than inheriting straight from IGameEventListener2.
// Avoids requiring the user to remove themselves as listeners in 
// their deconstructor, and sets the serverside variable based on
// our dll location.
class CGameEventListener : public IGameEventListener2
{
public:
	CGameEventListener() : m_bRegisteredForEvents(false)
	{
	}

	~CGameEventListener()
	{
		StopListeningForAllEvents();
	}

	void ListenForGameEvent(const char* name)
	{
		m_bRegisteredForEvents = true;

		bool bServerSide = false;

		if (I::GameEventManager)
		{
			I::GameEventManager->AddListener(this, name, bServerSide);
		}

		//Error("Failed to subscribe to event %s!", name);
	}

	void StopListeningForAllEvents()
	{
		// remove me from list
		if (m_bRegisteredForEvents)
		{
			if (I::GameEventManager)
				I::GameEventManager->RemoveListener(this);

			m_bRegisteredForEvents = false;
		}
	}

	// Intentionally abstract
	virtual void FireGameEvent(IGameEvent* event) = 0;

private:
	// Have we registered for any events?
	bool m_bRegisteredForEvents;
};

#endif