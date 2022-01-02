//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: generates 4 randum numbers in the range 0..1 quickly, using SIMD
//
//=====================================================================================//
#include "../public/tier0/basetypes.h"
#include "../public/mathlib/mathlib.h"
#include "../public/mathlib/vector.h"
#include "../public/mathlib/ssemath.h"

#include <math.h>
#include <float.h>
#include <memory.h>

// see knuth volume 3 for insight.

class SIMDRandStreamContext
{
	fltx4 m_RandY[55];

	fltx4* m_pRand_J, * m_pRand_K;


public:
	void Seed(uint32 seed)
	{
		m_pRand_J = m_RandY + 23; m_pRand_K = m_RandY + 54;
		for (int i = 0; i < 55; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				SubFloat(m_RandY[i], j) = (seed >> 16) / 65536.0;
				seed = (seed + 1) * 3141592621u;
			}
		}
	}

	inline fltx4 RandSIMD(void)
	{
		// ret= rand[k]+rand[j]
		fltx4 retval = AddSIMD(*m_pRand_K, *m_pRand_J);

		// if ( ret>=1.0) ret-=1.0
		fltx4 overflow_mask = CmpGeSIMD(retval, Four_Ones);
		retval = SubSIMD(retval, AndSIMD(Four_Ones, overflow_mask));

		*m_pRand_K = retval;

		// update pointers w/ wrap-around
		if (--m_pRand_J < m_RandY)
			m_pRand_J = m_RandY + 54;
		if (--m_pRand_K < m_RandY)
			m_pRand_K = m_RandY + 54;

		return retval;
	}
};

#define MAX_SIMULTANEOUS_RANDOM_STREAMS 32

static SIMDRandStreamContext s_SIMDRandContexts[MAX_SIMULTANEOUS_RANDOM_STREAMS];

static volatile int s_nRandContextsInUse[MAX_SIMULTANEOUS_RANDOM_STREAMS];

void SeedRandSIMD(uint32 seed)
{
	for (int i = 0; i < MAX_SIMULTANEOUS_RANDOM_STREAMS; i++)
		s_SIMDRandContexts[i].Seed(seed + i);
}

fltx4 RandSIMD(int nContextIndex)
{
	return s_SIMDRandContexts[nContextIndex].RandSIMD();
}

void ReleaseSIMDRandContext(int nContext)
{
	s_nRandContextsInUse[nContext] = 0;
}


fltx4 RandSIMD(void)
{
	return s_SIMDRandContexts[0].RandSIMD();
}