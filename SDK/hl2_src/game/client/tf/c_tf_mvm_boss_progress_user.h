//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
//
//
//=============================================================================
#ifndef C_TF_MVM_BOSS_HEALTH_USER_H
#define C_TF_MVM_BOSS_HEALTH_USER_H

#include "../../../public/tier0/platform.h"
#include "../../../public/tier1/utlvector.h"

abstract_class ITFMvMBossProgressUserAutoList
{
public:
	ITFMvMBossProgressUserAutoList(bool bAutoAdd = true);
	virtual ~ITFMvMBossProgressUserAutoList();

	static void AddToAutoList(ITFMvMBossProgressUserAutoList* pElement) { m_ITFMvMBossProgressUserAutoListAutoList.AddToTail(pElement); }
	static void RemoveFromAutoList(ITFMvMBossProgressUserAutoList* pElement) { m_ITFMvMBossProgressUserAutoListAutoList.FindAndFastRemove(pElement); }
	static const CUtlVector<ITFMvMBossProgressUserAutoList*>& AutoList(void) { return m_ITFMvMBossProgressUserAutoListAutoList; }

private:
	static CUtlVector<ITFMvMBossProgressUserAutoList*> m_ITFMvMBossProgressUserAutoListAutoList;
};

class C_TFMvMBossProgressUser : public ITFMvMBossProgressUserAutoList
{
public:
	virtual const char* GetBossProgressImageName() const { return NULL; }
	virtual float GetBossStatusProgress() const { return 0.f; }
};

#endif // C_TF_MVM_BOSS_HEALTH_USER_H