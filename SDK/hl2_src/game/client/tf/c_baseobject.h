//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Clients CBaseObject
//
// $NoKeywords: $
//=============================================================================//
#ifndef C_BASEOBJECT_H
#define C_BASEOBJECT_H
#ifdef _WIN32
#pragma once
#endif

#include "../c_basecombatcharacter.h"
#include "../../shared/hintsystem.h"
#include "../../shared/tf/ihasbuildpoints.h"
#include "../../shared/tf/tf_shareddefs.h"

// Max Length of ID Strings
#define MAX_ID_STRING		256

abstract_class IBaseObjectAutoList
{
public:
	IBaseObjectAutoList(bool bAutoAdd = true);
	virtual ~IBaseObjectAutoList();

	static void AddToAutoList(IBaseObjectAutoList* pElement) { m_IBaseObjectAutoList.AddToTail(pElement); }
	static void RemoveFromAutoList(IBaseObjectAutoList* pElement) { m_IBaseObjectAutoList.FindAndFastRemove(pElement); }
	static const CUtlVector<IBaseObjectAutoList*>& AutoList(void) { return m_IBaseObjectAutoList; }

private:
	static CUtlVector<IBaseObjectAutoList*> m_IBaseObjectAutoList;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class C_BaseObject : public C_BaseCombatCharacter, public IHasBuildPoints, public ITargetIDProvidesHint, public IBaseObjectAutoList
{
public:
	virtual						~C_BaseObject() = 0;

	virtual bool				IsAnUpgrade(void) const = 0;
	virtual void				SetType(int iObjectType) = 0;
	virtual void				Select(void) = 0;
	virtual	void				SetupAttachedVersion(void) = 0;
	virtual const char*			GetTargetDescription(void) const = 0;
	virtual const char*			GetIDString(void) = 0;
	virtual void				GetTargetIDString(wchar_t* sIDString, int iMaxLenInBytes, bool bSpectator) = 0;
	virtual void				GetTargetIDDataString(wchar_t* sDataString, int iMaxLenInBytes) = 0;
	virtual bool				ShouldBeActive(void) = 0;
	virtual void				OnGoActive(void) = 0;
	virtual void				OnGoInactive(void) = 0;
	virtual void				RecalculateIDString(void) = 0;
	virtual bool				IsUpgrading(void) const = 0;
	virtual void				FinishedBuilding(void) = 0;
	virtual const char*			GetStatusName() const = 0;
	virtual void				OnStartDisabled(void) = 0;
	virtual void				OnEndDisabled(void) = 0;
	virtual bool				ShouldPlayersAvoid(void) = 0;
	virtual bool				IsHostileUpgrade(void) = 0;
	virtual int					GetDisplayPriority(void) = 0;
	virtual const char*			GetHudStatusIcon(void) = 0;
	virtual int             	GetBuildingAlertLevel(void) = 0; //BuildingHudAlert_t
	virtual int					GetUpgradeLevel(void) = 0;
	virtual int					GetUpgradeMetalRequired(void) = 0;
	virtual void				UpgradeLevelChanged() = 0;
	virtual float				GetInvisibilityLevel(void) = 0;
	virtual void				SetInvisibilityLevel(float flValue) = 0;
	virtual int					AddBuildPoint(int iAttachmentNum) = 0;
	virtual void				AddValidObjectToBuildPoint(int iPoint, int iObjectType) = 0;
	virtual C_BaseObject*		GetBuildPointObject(int iPoint) = 0;
	virtual bool				IsPlacementPosValid(void) = 0;
	virtual float				GetNearbyObjectCheckRadius(void) = 0;
	virtual void				OnPlacementStateChanged(bool bValidPlacement) = 0;
	virtual void				UpdateDamageEffects(int damageLevel) = 0; //BuildingDamageLevel_t

public:
	M_NETVAR(m_iHealth, int, "CBaseObject", "m_iHealth");
	M_NETVAR(m_iMaxHealth, int, "CBaseObject", "m_iMaxHealth");
	M_NETVAR(m_bHasSapper, bool, "CBaseObject", "m_bHasSapper");
	M_NETVAR(m_iObjectType, int, "CBaseObject", "m_iObjectType");
	M_NETVAR(m_bBuilding, bool, "CBaseObject", "m_bBuilding");
	M_NETVAR(m_bPlacing, bool, "CBaseObject", "m_bPlacing");
	M_NETVAR(m_bCarried, bool, "CBaseObject", "m_bCarried");
	M_NETVAR(m_bCarryDeploy, bool, "CBaseObject", "m_bCarryDeploy");
	M_NETVAR(m_bMiniBuilding, bool, "CBaseObject", "m_bMiniBuilding");
	M_NETVAR(m_flPercentageConstructed, float, "CBaseObject", "m_flPercentageConstructed");
	M_NETVAR(m_fObjectFlags, int, "CBaseObject", "m_fObjectFlags");
	M_NETVAR(m_hBuiltOnEntity, EHANDLE, "CBaseObject", "m_hBuiltOnEntity");
	M_NETVAR(m_bDisabled, bool, "CBaseObject", "m_bDisabled");
	M_NETVAR(m_hBuilder, EHANDLE, "CBaseObject", "m_hBuilder");
	M_NETVAR(m_vecBuildMaxs, Vector, "CBaseObject", "m_vecBuildMaxs");
	M_NETVAR(m_vecBuildMins, Vector, "CBaseObject", "m_vecBuildMins");
	M_NETVAR(m_iDesiredBuildRotations, int, "CBaseObject", "m_iDesiredBuildRotations");
	M_NETVAR(m_bServerOverridePlacement, bool, "CBaseObject", "m_bServerOverridePlacement");
	M_NETVAR(m_iUpgradeLevel, int, "CBaseObject", "m_iUpgradeLevel");
	M_NETVAR(m_iUpgradeMetal, int, "CBaseObject", "m_iUpgradeMetal");
	M_NETVAR(m_iUpgradeMetalRequired, int, "CBaseObject", "m_iUpgradeMetalRequired");
	M_NETVAR(m_iHighestUpgradeLevel, int, "CBaseObject", "m_iHighestUpgradeLevel");
	M_NETVAR(m_iObjectMode, int, "CBaseObject", "m_iObjectMode");
	M_NETVAR(m_bDisposableBuilding, bool, "CBaseObject", "m_bDisposableBuilding");
	M_NETVAR(m_bWasMapPlaced, bool, "CBaseObject", "m_bWasMapPlaced");
	M_NETVAR(m_bPlasmaDisable, bool, "CBaseObject", "m_bPlasmaDisable");

public:
	inline bool IsDisabled()
	{
		return (m_bDisabled() || m_bHasSapper());
	}
};

#endif //C_BASEOBJECT_H