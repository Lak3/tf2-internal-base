//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//=============================================================================//
#if !defined(_STATIC_LINKED) || defined(_SHARED_LIB)

#include <malloc.h>
#include "vallocator.h"
#include "tier0/basetypes.h"

void* VStdAllocator::Alloc(unsigned long size)
{
	if (size)
	{
		void* ret = malloc(size);
		return ret;
	}
	else
	{
		return nullptr;
	}
}

void VStdAllocator::Free(void* ptr)
{
	free(ptr);
}

#endif // !_STATIC_LINKED || _SHARED_LIB