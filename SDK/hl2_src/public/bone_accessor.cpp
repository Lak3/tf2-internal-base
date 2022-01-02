//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#include "bone_accessor.h"

#include "../game/client/c_baseanimating.h"

#if defined( _DEBUG )

/*
void CBoneAccessor::SanityCheckBone(int iBone, bool bReadable) const
{
	if (m_pAnimating)
	{
		CStudioHdr* pHdr = nullptr; //m_pAnimating->GetModelPtr();
		if (pHdr)
		{
			mstudiobone_t* pBone = pHdr->pBone(iBone);
			if (bReadable)
			{
				assert(pBone->flags & m_ReadableBones);
			}
			else
			{
				assert(pBone->flags & m_WritableBones);
			}
		}
	}
}
*/

#endif