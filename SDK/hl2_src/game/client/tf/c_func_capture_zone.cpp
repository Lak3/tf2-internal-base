//========= Copyright(c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#include "c_func_capture_zone.h"

CUtlVector<ICaptureZoneAutoList*> ICaptureZoneAutoList::m_ICaptureZoneAutoList;

ICaptureZoneAutoList::ICaptureZoneAutoList(bool bAutoAdd)
{
	if (bAutoAdd)
	{
		AddToAutoList(this);
	}
}

ICaptureZoneAutoList::~ICaptureZoneAutoList()
{
	RemoveFromAutoList(this);
}