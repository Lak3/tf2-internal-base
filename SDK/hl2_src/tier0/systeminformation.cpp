//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#pragma once

#include "../public/tier0/platform.h"

#ifdef IS_WINDOWS_PC 

//
//	Plat_GetMemPageSize
//		Returns the size of a memory page in bytes.
//
unsigned long Plat_GetMemPageSize()
{
	return 4;	// On 32-bit systems memory page size is 4 Kb
}

#else

//
//	Plat_GetMemPageSize
//		Returns the size of a memory page in bytes.
//
unsigned long Plat_GetMemPageSize()
{
	return 4;	// Assume unknown page size is 4 Kb
}

#endif