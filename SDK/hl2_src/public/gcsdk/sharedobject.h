//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Base class for objects that are kept in synch between client and server
//
//=============================================================================
#ifndef SHAREDOBJECT_H
#define SHAREDOBJECT_H
#ifdef _WIN32
#pragma once
#endif

// ENABLE_SO_OVERWRITE_PARANOIA can be set to either 0 or 1. If enabled, it will add
// extra fields to every CSharedObject instance to try and detect overwrites at the
// cost of additional runtime memory.
#define ENABLE_SO_OVERWRITE_PARANOIA				0

// ENABLE_SO_CONSTRUCT_DESTRUCT_PARANOIA can be set to either 0 or 1. If enabled, it
// will add extra fields to every CSharedObject instance to try and detect issues with
// constructions/destruction (ie., double-deletes, etc.), including reference counting.
#define ENABLE_SO_CONSTRUCT_DESTRUCT_PARANOIA		(defined( STAGING_ONLY ))

namespace GCSDK
{
	class CSQLAccess;
	class CSharedObject;
	typedef CSharedObject* (*SOCreationFunc_t)();
	class CSharedObjectCache;
} // namespace GCSDK

#endif //SHAREDOBJECT_H