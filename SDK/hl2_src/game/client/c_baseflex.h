//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//===========================================================================//
#ifndef C_STUDIOFLEX_H
#define C_STUDIOFLEX_H
#pragma once

#include "c_baseanimating.h"
#include "c_baseanimatingoverlay.h"

//-----------------------------------------------------------------------------
// Purpose: Item in list of loaded scene files
//-----------------------------------------------------------------------------
class CFlexSceneFile
{
public:
	enum
	{
		MAX_FLEX_FILENAME = 128,
	};

	char			filename[MAX_FLEX_FILENAME];
	void* buffer;
};

// For phoneme emphasis track
struct Emphasized_Phoneme;

class CSentence;
class CChoreoScene;
class CSceneEventInfo;
class CChoreoEvent;
class CChoreoActor;

enum
{
	PHONEME_CLASS_WEAK = 0,
	PHONEME_CLASS_NORMAL,
	PHONEME_CLASS_STRONG,

	NUM_PHONEME_CLASSES
};

// Mapping for each loaded scene file used by this actor
struct FS_LocalToGlobal_t
{
	explicit FS_LocalToGlobal_t() :
		m_Key(0),
		m_nCount(0),
		m_Mapping(0)
	{
	}

	explicit FS_LocalToGlobal_t(const flexsettinghdr_t* key) :
		m_Key(key),
		m_nCount(0),
		m_Mapping(0)
	{
	}

	void SetCount(int count)
	{
		assert(!m_Mapping);
		assert(count > 0);
		m_nCount = count;
		m_Mapping = new int[m_nCount];
		Q_memset(m_Mapping, 0, m_nCount * sizeof(int));
	}

	FS_LocalToGlobal_t(const FS_LocalToGlobal_t& src)
	{
		m_Key = src.m_Key;
		delete m_Mapping;
		m_Mapping = new int[src.m_nCount];
		Q_memcpy(m_Mapping, src.m_Mapping, src.m_nCount * sizeof(int));

		m_nCount = src.m_nCount;
	}

	~FS_LocalToGlobal_t()
	{
		delete m_Mapping;
		m_nCount = 0;
		m_Mapping = 0;
	}

	const flexsettinghdr_t* m_Key;
	int	 m_nCount;
	int* m_Mapping;
};

//-----------------------------------------------------------------------------
// Purpose: Each CBaseFlex maintains a UtlRBTree of mappings, one for each loaded flex scene file it uses.  This is used to
//  sort the entries in the RBTree
// Input  : lhs - 
//			rhs - 
// Output : Returns true on success, false on failure.
//-----------------------------------------------------------------------------
inline bool FlexSettingLessFunc(const FS_LocalToGlobal_t& lhs, const FS_LocalToGlobal_t& rhs)
{
	return lhs.m_Key < rhs.m_Key;
}

class IHasLocalToGlobalFlexSettings
{
public:
	virtual void		EnsureTranslations(const flexsettinghdr_t* pSettinghdr) = 0;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
struct Emphasized_Phoneme
{
	// Global fields, setup at start
	char			classname[64];
	bool			required;
	// Global fields setup first time tracks played
	bool			basechecked;
	const flexsettinghdr_t* base;
	const flexsetting_t* exp;

	// Local fields, processed for each sentence
	bool			valid;
	float			amount;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_BaseFlex : public C_BaseAnimatingOverlay, public IHasLocalToGlobalFlexSettings
{
public:
	virtual			    ~C_BaseFlex() = 0;

	virtual void		InitPhonemeMappings() = 0;
	virtual	bool	    SetupGlobalWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual	void	    SetupLocalWeights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual Vector	    SetViewTarget(CStudioHdr* pStudioHdr) = 0;
	virtual bool		StartSceneEvent(CSceneEventInfo* info, CChoreoScene* scene, CChoreoEvent* event, CChoreoActor* actor, C_BaseEntity* pTarget) = 0;
	virtual void		ProcessSceneEvents(bool bFlexEvents) = 0;
	virtual	bool		ProcessSceneEvent(bool bFlexEvents, CSceneEventInfo* info, CChoreoScene* scene, CChoreoEvent* event) = 0;
	virtual bool		ProcessSequenceSceneEvent(CSceneEventInfo* info, CChoreoScene* scene, CChoreoEvent* event) = 0;
	virtual	bool		ClearSceneEvent(CSceneEventInfo* info, bool fastKill, bool canceled) = 0;
	virtual bool		CheckSceneEventCompletion(CSceneEventInfo* info, float currenttime, CChoreoScene* scene, CChoreoEvent* evnt) = 0;

public:
	M_NETVAR(m_blinktoggle, bool, "CBaseFlex", "m_blinktoggle");
	M_NETVAR(m_viewtarget, Vector, "CBaseFlex", "m_viewtarget");

public:
	inline std::array<float, MAXSTUDIOFLEXCTRL>& m_flexWeight()
	{
		static const int nOffset = GetNetVar("CBaseFlex", "m_flexWeight");
		return *reinterpret_cast<std::array<float, MAXSTUDIOFLEXCTRL>*>(reinterpret_cast<DWORD>(this) + nOffset);
	}
};

#endif //C_STUDIOFLEX_H