//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: This header should never be used directly from leaf code!!!
// Instead, just add the file memoverride.cpp into your project and all this
// will automagically be used
//
// $NoKeywords: $
//=============================================================================//
#ifndef TIER0_MEMALLOC_H
#define TIER0_MEMALLOC_H
#ifdef _WIN32
#pragma once
#endif

#include "commonmacros.h"
#include <malloc.h>
#include <string>

// These memory debugging switches aren't relevant under Linux builds since memoverride.cpp
// isn't built into Linux projects
#ifndef POSIX
// Define this in release to get memory tracking even in release builds
//#define USE_MEM_DEBUG 1
#endif

#if defined( _MEMTEST )
#ifdef _WIN32
#define USE_MEM_DEBUG 1
#endif
#endif

// Undefine this if using a compiler lacking threadsafe RTTI (like vc6)
#define MEM_DEBUG_CLASSNAME 1

typedef size_t(*MemAllocFailHandler_t)(size_t);

inline void* MemAlloc_Alloc(size_t nSize)
{
	return malloc(nSize);
}

inline void MemAlloc_Free(void* ptr)
{
	free(ptr);
}

inline bool ValueIsPowerOfTwo(size_t value)			// don't clash with mathlib definition
{
	return (value & (value - 1)) == 0;
}

inline void* MemAlloc_AllocAligned(size_t size, size_t align)
{
	unsigned char* pAlloc, * pResult;

	if (!IsPowerOfTwo(align))
		return NULL;

	align = (align > sizeof(void*) ? align : sizeof(void*)) - 1;

	if ((pAlloc = (unsigned char*)malloc(sizeof(void*) + align + size)) == (unsigned char*)NULL)
		return NULL;

	pResult = (unsigned char*)((size_t)(pAlloc + sizeof(void*) + align) & ~align);
	((unsigned char**)(pResult))[-1] = pAlloc;

	return (void*)pResult;
}

inline void* MemAlloc_AllocAlignedUnattributed(size_t size, size_t align)
{
	unsigned char* pAlloc, * pResult;

	if (!ValueIsPowerOfTwo(align))
		return NULL;

	align = (align > sizeof(void*) ? align : sizeof(void*)) - 1;

	if ((pAlloc = (unsigned char*)MemAlloc_Alloc(sizeof(void*) + align + size)) == (unsigned char*)NULL)
		return NULL;

	pResult = (unsigned char*)((size_t)(pAlloc + sizeof(void*) + align) & ~align);
	((unsigned char**)(pResult))[-1] = pAlloc;

	return (void*)pResult;
}

inline void* MemAlloc_ReallocAligned(void* ptr, size_t size, size_t align)
{
	if (!IsPowerOfTwo(align))
		return NULL;

	// Don't change alignment between allocation + reallocation.
	if (((size_t)ptr & (align - 1)) != 0)
		return NULL;

	if (!ptr)
		return MemAlloc_AllocAligned(size, align);

	void* pAlloc, * pResult;

	// Figure out the actual allocation point
	pAlloc = ptr;
	pAlloc = (void*)(((size_t)pAlloc & ~(sizeof(void*) - 1)) - sizeof(void*));
	pAlloc = *((void**)pAlloc);

	// See if we have enough space
	size_t nOffset = (size_t)ptr - (size_t)pAlloc;
	size_t nOldSize = _msize(pAlloc);
	if (nOldSize >= size + nOffset)
		return ptr;

	pResult = MemAlloc_AllocAligned(size, align);
	memcpy(pResult, ptr, nOldSize - nOffset);
	free(pAlloc);
	return pResult;
}

inline void MemAlloc_FreeAligned(void* pMemBlock)
{
	void* pAlloc;

	if (pMemBlock == NULL)
		return;

	pAlloc = pMemBlock;

	// pAlloc points to the pointer to starting of the memory block
	pAlloc = (void*)(((size_t)pAlloc & ~(sizeof(void*) - 1)) - sizeof(void*));

	// pAlloc is the pointer to the start of memory block
	pAlloc = *((void**)pAlloc);
	free(pAlloc);
}

inline size_t MemAlloc_GetSizeAligned(void* pMemBlock)
{
	void* pAlloc;

	if (pMemBlock == NULL)
		return 0;

	pAlloc = pMemBlock;

	// pAlloc points to the pointer to starting of the memory block
	pAlloc = (void*)(((size_t)pAlloc & ~(sizeof(void*) - 1)) - sizeof(void*));

	// pAlloc is the pointer to the start of memory block
	pAlloc = *((void**)pAlloc);
	return _msize(pAlloc) - ((unsigned char*)pMemBlock - (unsigned char*)pAlloc);
}

struct aligned_tmp_t
{
	// empty base class
};

/*
This class used to be required if you wanted an object to be allocated with a specific
alignment. ALIGN16 and ALIGN16_POST are not actually sufficient for this because they
guarantee that the globals, statics, locals, and function parameters are appropriately
aligned they do not affect memory allocation alignment.
However this class is usually not needed because as of 2012 our policy is that our
allocator should take care of this automatically. Any object whose size is a multiple
of 16 will be 16-byte aligned. Existing uses of this class were not changed because
the cost/benefit did not justify it.
*/
// template here to allow adding alignment at levels of hierarchy that aren't the base
template< int bytesAlignment = 16, class T = aligned_tmp_t >
class CAlignedNewDelete : public T
{
public:
	/*
	Note that this class does not overload operator new[] and delete[] which means that
	classes that depend on this for alignment may end up misaligned if an array is
	allocated. This problem is now mostly theoretical because this class is mostly
	obsolete.
	*/
	void* operator new(size_t nSize)
	{
		return MemAlloc_AllocAligned(nSize, bytesAlignment);
	}

	void operator delete(void* pData)
	{
		if (pData)
		{
			MemAlloc_FreeAligned(pData);
		}
	}

	//void operator delete(void* pData, int nBlockUse, const char* pFileName, int nLine)
	//{
	//	if (pData)
	//	{
	//		MemAlloc_FreeAligned(pData, pFileName, nLine);
	//	}
	//}
};

#endif