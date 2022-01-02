//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#if !defined( HUDELEMENT_H )
#define HUDELEMENT_H
#ifdef _WIN32
#pragma once
#endif

#include "hud.h"
#include "../shared/GameEventListener.h"

#ifdef new
#undef new
#endif

//-----------------------------------------------------------------------------
// Purpose: Base class for all hud elements
//-----------------------------------------------------------------------------
class CHudElement : public CGameEventListener
{
public:
	// memory handling, uses calloc so members are zero'd out on instantiation
	void* operator new(size_t stAllocateBlock)
	{
		assert(stAllocateBlock != 0);
		void* pMem = malloc(stAllocateBlock);
		memset(pMem, 0, stAllocateBlock);
		return pMem;
	}

	void operator delete(void* pMem)
	{
#if defined( _DEBUG )
		int size = _msize(pMem);
		memset(pMem, 0xcd, size);
#endif
		free(pMem);
	}

	void operator delete(void* pMem, int nBlockUse, const char* pFileName, int nLine)
	{
		operator delete(pMem);
	}

public: // IGameEventListener Interface
	virtual void FireGameEvent(IGameEvent* event) { }

	// True if this element is visible, and should think
	bool						m_bActive;

	int							m_iHiddenBits;

	const char* m_pElementName;
	bool						m_bNeedsRemove;
	bool						m_bIsParentedToClientDLLRootPanel;

	CUtlVector< int >			m_HudRenderGroups;
};

#endif // HUDELEMENT_H