//========= Copyright(c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#ifndef C_FUNC_CAPTURE_ZONE_H
#define C_FUNC_CAPTURE_ZONE_H
#ifdef _WIN32
#pragma once
#endif

#include "../c_baseentity.h"

class C_CaptureZone;

abstract_class ICaptureZoneAutoList
{
public:
	ICaptureZoneAutoList(bool bAutoAdd = true);
	virtual ~ICaptureZoneAutoList();

	static void AddToAutoList(ICaptureZoneAutoList* pElement) { m_ICaptureZoneAutoList.AddToTail(pElement); }
	static void RemoveFromAutoList(ICaptureZoneAutoList* pElement) { m_ICaptureZoneAutoList.FindAndFastRemove(pElement); }
	static const CUtlVector<ICaptureZoneAutoList*>& AutoList(void) { return m_ICaptureZoneAutoList; }

private:
	static CUtlVector<ICaptureZoneAutoList*> m_ICaptureZoneAutoList;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_CaptureZone : public C_BaseEntity, public ICaptureZoneAutoList
{
public:
	M_NETVAR(m_bDisabled, bool, "CCaptureZone", "m_bDisabled");
};

#endif //C_FUNC_CAPTURE_ZONE_H