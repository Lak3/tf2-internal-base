//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#ifndef VSTDLIB_H
#define VSTDLIB_H
#ifdef _WIN32
#pragma once
#endif

#include "../tier0/platform.h"

//-----------------------------------------------------------------------------
// dll export stuff
//-----------------------------------------------------------------------------
#ifdef VSTDLIB_DLL_EXPORT
#define VSTDLIB_INTERFACE
#define VSTDLIB_OVERLOAD
#define VSTDLIB_CLASS
#define VSTDLIB_GLOBAL
#else
#define VSTDLIB_INTERFACE
#define VSTDLIB_OVERLOAD
#define VSTDLIB_CLASS
#define VSTDLIB_GLOBAL
#endif

#endif // VSTDLIB_H