//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#ifndef ECON_ENTITY_H
#define ECON_ENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "ihasattributes.h"
#include "ihasowner.h"

#include "../ai_activity.h"
#include "../../client/c_baseanimating.h"

#define USES_ECON_ITEMS

class C_BaseViewModel;
class CStudioHdr;
class CMaterialReference;
class C_BaseCombatCharacter;

// Additional attachments.
struct AttachedModelData_t
{
	const model_t* m_pModel;
	int m_iModelDisplayFlags;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class C_EconEntity : public C_BaseAnimating, public IHasAttributes
{
public:
	virtual					~C_EconEntity() = 0;

	virtual bool			ShouldShowToolTip(void) = 0;
	virtual bool			OnFireEvent(C_BaseViewModel* pViewModel, const Vector& origin, const QAngle& angles, int evnt, const char* options) = 0;
	virtual bool			ShouldDrawParticleSystems(void) = 0;
	virtual void			UpdateAttachmentModels(void) = 0;
	virtual bool			ViewModel_IsTransparent(void) = 0;
	virtual bool			ViewModel_IsUsingFBTexture(void) = 0;
	virtual bool			IsOverridingViewmodel(void) = 0;
	virtual int				DrawOverriddenViewmodel(C_BaseViewModel* pViewmodel, int flags) = 0;
	virtual bool			GetAttachment(const char* szName, Vector& absOrigin) = 0;
	virtual bool			GetAttachment(const char* szName, Vector& absOrigin, QAngle& absAngles) = 0;
	virtual void			ViewModelAttachmentBlending(CStudioHdr* hdr, Vector pos[], Quaternion q[], float currentTime, int boneMask) = 0;
	virtual void			SetMaterialOverride(int team, const char* pszMaterial) = 0;
	virtual void			SetMaterialOverride(int team, CMaterialReference& ref) = 0;
	virtual bool			UpdateBodygroups(C_BaseCombatCharacter* pOwner, int iState) = 0;
	virtual Activity		TranslateViewmodelHandActivityInternal(Activity actBase) = 0;
	
public:
	M_NETVAR(m_AttributeManager, void*, "CEconEntity", "m_AttributeManager");
	M_NETVAR(m_hOuter, int, "CEconEntity", "m_hOuter");
	M_NETVAR(m_ProviderType, int, "CEconEntity", "m_ProviderType");
	M_NETVAR(m_iReapplyProvisionParity, int, "CEconEntity", "m_iReapplyProvisionParity");
	M_NETVAR(m_Item, void*, "CEconEntity", "m_Item");
	M_NETVAR(m_iItemDefinitionIndex, uint16, "CEconEntity", "m_iItemDefinitionIndex");
	M_NETVAR(m_iEntityLevel, int, "CEconEntity", "m_iEntityLevel");
	M_NETVAR(m_iItemIDHigh, int, "CEconEntity", "m_iItemIDHigh");
	M_NETVAR(m_iItemIDLow, int, "CEconEntity", "m_iItemIDLow");
	M_NETVAR(m_iAccountID, int, "CEconEntity", "m_iAccountID");
	M_NETVAR(m_iEntityQuality, int, "CEconEntity", "m_iEntityQuality");
	M_NETVAR(m_bInitialized, bool, "CEconEntity", "m_bInitialized");
	M_NETVAR(m_bOnlyIterateItemViewAttributes, bool, "CEconEntity", "m_bOnlyIterateItemViewAttributes");
	M_NETVAR(m_AttributeList, void*, "CEconEntity", "m_AttributeList");
	M_NETVAR(m_iAttributeDefinitionIndex, int, "CEconEntity", "m_iAttributeDefinitionIndex");
	M_NETVAR(m_flValue, float, "CEconEntity", "m_flValue");
	M_NETVAR(m_nRefundableCurrency, int, "CEconEntity", "m_nRefundableCurrency");
	M_NETVAR(m_iTeamNumber, int, "CEconEntity", "m_iTeamNumber");
	M_NETVAR(m_NetworkedDynamicAttributesForDemos, void*, "CEconEntity", "m_NetworkedDynamicAttributesForDemos");
	M_NETVAR(m_bValidatedAttachedEntity, bool, "CEconEntity", "m_bValidatedAttachedEntity");
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
/*
class C_EconEntity : public C_BaseAnimating, public IHasAttributes
{
public:
	M_NETVAR(m_AttributeManager, void*, "CEconEntity", "m_AttributeManager");
	M_NETVAR(m_hOuter, int, "CEconEntity", "m_hOuter");
	M_NETVAR(m_ProviderType, int, "CEconEntity", "m_ProviderType");
	M_NETVAR(m_iReapplyProvisionParity, int, "CEconEntity", "m_iReapplyProvisionParity");
	M_NETVAR(m_Item, void*, "CEconEntity", "m_Item");
	M_NETVAR(m_iItemDefinitionIndex, uint16, "CEconEntity", "m_iItemDefinitionIndex");
	M_NETVAR(m_iEntityLevel, int, "CEconEntity", "m_iEntityLevel");
	M_NETVAR(m_iItemIDHigh, int, "CEconEntity", "m_iItemIDHigh");
	M_NETVAR(m_iItemIDLow, int, "CEconEntity", "m_iItemIDLow");
	M_NETVAR(m_iAccountID, int, "CEconEntity", "m_iAccountID");
	M_NETVAR(m_iEntityQuality, int, "CEconEntity", "m_iEntityQuality");
	M_NETVAR(m_bInitialized, bool, "CEconEntity", "m_bInitialized");
	M_NETVAR(m_bOnlyIterateItemViewAttributes, bool, "CEconEntity", "m_bOnlyIterateItemViewAttributes");
	M_NETVAR(m_AttributeList, void*, "CEconEntity", "m_AttributeList");
	M_NETVAR(m_iAttributeDefinitionIndex, int, "CEconEntity", "m_iAttributeDefinitionIndex");
	M_NETVAR(m_flValue, float, "CEconEntity", "m_flValue");
	M_NETVAR(m_nRefundableCurrency, int, "CEconEntity", "m_nRefundableCurrency");
	M_NETVAR(m_iTeamNumber, int, "CEconEntity", "m_iTeamNumber");
	M_NETVAR(m_NetworkedDynamicAttributesForDemos, void*, "CEconEntity", "m_NetworkedDynamicAttributesForDemos");
	M_NETVAR(m_bValidatedAttachedEntity, bool, "CEconEntity", "m_bValidatedAttachedEntity");
};
*/

#endif //ECON_ENTITY_H