//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#ifndef IHASOWNER_H
#define IHASOWNER_H
#ifdef _WIN32
#pragma once
#endif

class C_BaseEntity;

//-----------------------------------------------------------------------------
// Purpose: Allows an entity to access its owner regardless of entity type 
//-----------------------------------------------------------------------------
class IHasOwner
{
public:
	virtual C_BaseEntity* GetOwnerViaInterface(void) = 0;
};

#endif // IHASOWNER_H
