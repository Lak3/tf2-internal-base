//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
//
//
//=============================================================================
#include "c_tf_mvm_boss_progress_user.h"

CUtlVector<ITFMvMBossProgressUserAutoList*> ITFMvMBossProgressUserAutoList::m_ITFMvMBossProgressUserAutoListAutoList;

ITFMvMBossProgressUserAutoList::ITFMvMBossProgressUserAutoList(bool bAutoAdd)
{
	if (bAutoAdd)
	{
		AddToAutoList(this);
	}
}

ITFMvMBossProgressUserAutoList::~ITFMvMBossProgressUserAutoList()
{
	RemoveFromAutoList(this);
}