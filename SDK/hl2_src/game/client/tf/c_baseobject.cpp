//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Clients CBaseObject
//
// $NoKeywords: $
//=============================================================================//
#include "c_baseobject.h"

CUtlVector<IBaseObjectAutoList*> IBaseObjectAutoList::m_IBaseObjectAutoList;

IBaseObjectAutoList::IBaseObjectAutoList(bool bAutoAdd)
{
	if (bAutoAdd)
	{
		AddToAutoList(this);
	}
}

IBaseObjectAutoList::~IBaseObjectAutoList()
{
	RemoveFromAutoList(this);
}