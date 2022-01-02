//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#include "bonelist.h"

CBoneList::CBoneList()
{
	m_bShouldDelete = false;
	m_nBones = 0;
	Q_memset(m_vecPos, 0, sizeof(m_vecPos));
	Q_memset(m_quatRot, 0, sizeof(m_quatRot));
}

void CBoneList::Release()
{
	if (m_bShouldDelete)
	{
		delete this;
	}
	else
	{
		Warning(L"Called Release() on CBoneList not allocated via Alloc() method\n");
	}
}

CBoneList* CBoneList::Alloc()
{
	CBoneList* newList = new CBoneList;
	assert(newList);

	if (newList)
	{
		newList->m_bShouldDelete = true;
	}

	return newList;
}

CFlexList::CFlexList()
{
	m_bShouldDelete = false;
	m_nNumFlexes = 0;
	Q_memset(m_flexWeights, 0, sizeof(m_flexWeights));
}

void CFlexList::Release()
{
	if (m_bShouldDelete)
	{
		delete this;
	}
	else
	{
		Warning(L"Called Release() on CFlexList not allocated via Alloc() method\n");
	}
}

CFlexList* CFlexList::Alloc()
{
	CFlexList* newList = new CFlexList;
	assert(newList);

	if (newList)
	{
		newList->m_bShouldDelete = true;
	}

	return newList;
}