//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
//	Weapons.
//
//	CTFWeaponBase
//	|
//	|--> CTFWeaponBaseMelee
//	|		|
//	|		|--> CTFWeaponCrowbar
//	|		|--> CTFWeaponKnife
//	|		|--> CTFWeaponMedikit
//	|		|--> CTFWeaponWrench
//	|
//	|--> CTFWeaponBaseGrenade
//	|		|
//	|		|--> CTFWeapon
//	|		|--> CTFWeapon
//	|
//	|--> CTFWeaponBaseGun
//
//=============================================================================//
#ifndef TF_WEAPONBASE_H
#define TF_WEAPONBASE_H
#ifdef _WIN32
#pragma once
#endif

#include "../basecombatweapon_shared.h"
#include "../econ/ihasowner.h"
#include "../GameEventListener.h"

#define MAX_TRACER_NAME 128

class CTFPlayer;
class CBaseObject;
class CTFWeaponBaseGrenadeProj;
class CTFWeaponAttachmentModel;
class CTFWeaponInfo;

// Reloading singly.
enum
{
	TF_RELOAD_START = 0,
	TF_RELOADING,
	TF_RELOADING_CONTINUE,
	TF_RELOAD_FINISH
};

// structure to encapsulate state of head bob
struct BobState_t
{
	BobState_t()
	{
		m_flBobTime = 0;
		m_flLastBobTime = 0;
		m_flLastSpeed = 0;
		m_flVerticalBob = 0;
		m_flLateralBob = 0;
	}

	float m_flBobTime;
	float m_flLastBobTime;
	float m_flLastSpeed;
	float m_flVerticalBob;
	float m_flLateralBob;
};

enum EWeaponStrangeType_t
{
	STRANGE_UNKNOWN = -1,
	STRANGE_NOT_STRANGE = 0,
	STRANGE_IS_STRANGE = 1,
};

enum EWeaponStatTrakModuleType_t
{
	MODULE_UNKNOWN = -1,
	MODULE_NONE = 0,
	MODULE_FOUND = 1,
};

// Interface for weapons that have a charge time
class ITFChargeUpWeapon
{
public:
	virtual bool  CanCharge(void) = 0;

	virtual float GetChargeBeginTime(void) = 0;
	virtual float GetChargeMaxTime(void) = 0;

	virtual float GetCurrentCharge(void)
	{
		return (I::GlobalVars->curtime - GetChargeBeginTime()) / GetChargeMaxTime();
	}
};

#define ENERGY_WEAPON_MAX_CHARGE 20

#define TF_PARTICLE_WEAPON_BLUE_1 Vector( 0.345, 0.52, 0.635 )
#define TF_PARTICLE_WEAPON_BLUE_2 Vector( 0.145, 0.427, 0.55 )
#define TF_PARTICLE_WEAPON_RED_1  Vector( 0.72, 0.22, 0.23 )
#define TF_PARTICLE_WEAPON_RED_2  Vector( 0.5, 0.18, 0.125 )

//=============================================================================
//
// Base TF Weapon Class
//
//=============================================================================
class C_TFWeaponBase : public C_BaseCombatWeapon, public IHasOwner, public CGameEventListener
{
public:
	virtual						~C_TFWeaponBase() = 0;

	virtual void				FallInit(void) = 0;
	virtual int					GetWeaponID(void) const = 0;
	virtual int					GetDamageType() const = 0;
	virtual int					GetCustomDamageType() const = 0;
	virtual bool				UsesPrimaryAmmo() = 0;
	virtual float				UberChargeAmmoPerShot(void) = 0;

private:
	virtual void*				GetPlayerPoseParamList(int& nUnknown) = 0;
	virtual void*				GetItemPoseParamList(int& nUnknown) = 0;

public:
	virtual void				StartHolsterAnim(void) = 0;
	virtual void				UpdateHands(void) = 0;
	virtual bool				OwnerCanTaunt(void) = 0;
	virtual bool				CanBeCritBoosted(void) = 0;

private:
	virtual void*				GetTauntItem(void) const = 0;

public:
	virtual void				RemoveExtraWearables(void) = 0;
	virtual void				Misfire(void) = 0;
	virtual void				FireFullClipAtOnce(void) = 0;
	virtual bool				CalcIsAttackCriticalHelper() = 0;
	virtual bool				CalcIsAttackCriticalHelperNoCrits() = 0;
	virtual ETFDmgCustom		GetPenetrateType() const = 0;
	virtual void				GetProjectileFireSetup(CTFPlayer* pPlayer, Vector vecOffset, Vector* vecSrc, QAngle* angForward, bool bHitTeammates = true, float flEndDist = 2000.f) = 0;
	virtual QAngle				GetSpreadAngles(void) = 0;
	virtual bool				IsFiring(void) const = 0;
	virtual bool				AreRandomCritsEnabled(void) = 0;
	virtual bool				DefaultReload(int iClipSize1, int iClipSize2, int iActivity) = 0;
	virtual bool				IsReloading() const = 0;
	virtual float				GetReloadSpeedScale(void) const = 0;
	virtual bool				CheckReloadMisfire(void) = 0;
	virtual bool				CanDrop(void) = 0;
	virtual bool				AllowTaunts(void) = 0;
	virtual float				ApplyFireDelay(float flDelay) const = 0;
	virtual int					GetActivityWeaponRole() const = 0;
	virtual int					GetViewModelWeaponRole() = 0;
	virtual bool				ShouldPlayClientReloadSound() = 0;
	virtual bool				CanAttack() = 0;
	virtual int					GetCanAttackFlags() const = 0;
	virtual void				WeaponReset(void) = 0;
	virtual void				WeaponRegenerate(void) = 0;
	virtual const char*			GetMuzzleFlashEffectName_3rd(void) = 0;
	virtual const char*			GetMuzzleFlashEffectName_1st(void) = 0;
	virtual const char*			GetMuzzleFlashModel(void) = 0;
	virtual float				GetMuzzleFlashModelLifetime(void) = 0;
	virtual const char*			GetMuzzleFlashParticleEffect(void) = 0;
	virtual void				Operator_HandleAnimEvent(void* pEvent, C_BaseCombatCharacter* pOperator) = 0; //animevent_t
	virtual const char*			GetInventoryModel(void) = 0;
	virtual float				GetSpeedMod(void) = 0;
	virtual bool				CanFireCriticalShot(bool bIsHeadshot = false) = 0; //423
	virtual bool				CanFireRandomCriticalShot(float flCritChance) = 0;
	virtual void				OnControlStunned(void) = 0;
	virtual bool				HideWhileStunned(void) = 0;
	virtual bool				IsViewModelFlipped(void) = 0;
	virtual int					GetMaxHealthMod() = 0;
	virtual float				GetLastDeployTime(void) = 0;
	virtual bool				IsEnergyWeapon(void) const = 0;
	virtual bool				IsBlastImpactWeapon(void) const = 0;
	virtual float				Energy_GetShotCost(void) const = 0;
	virtual float				Energy_GetRechargeCost(void) const = 0;
	virtual Vector				GetParticleColor(int iColor) = 0;
	virtual bool				HasLastShotCritical(void) = 0;
	virtual bool				UseServerRandomSeed(void) const = 0;
	virtual bool				IsBroken(void) const = 0;
	virtual void				SetBroken(bool bState) = 0;
	virtual void				DispatchMuzzleFlash(const char* effectName, C_BaseEntity* pAttachEnt) = 0;
	virtual bool				ShouldEjectBrass() = 0;
	virtual const char*			GetEffectLabelText() = 0;
	virtual float				GetProgress() = 0;
	virtual float				GetEffectBarProgress(void) = 0;
	virtual bool				CanPickupOtherWeapon() const = 0;
	virtual bool				ShouldRemoveInvisibilityOnPrimaryAttack() const = 0;
	virtual int					GetEffectBarAmmo(void) = 0;
	virtual float				InternalGetEffectBarRechargeTime(void) = 0;
	virtual void				CreateMuzzleFlashEffects(C_BaseEntity* pAttachEnt, int nIndex) = 0;
	virtual void				UpdateExtraWearablesVisibility() = 0;
	virtual bool				CanInspect() const = 0;

private:
	virtual void*				HookAttributes(void) = 0;
	virtual void*				OnUpgraded(void) = 0;

public:
	M_NETVAR(m_bLowered, bool, "CTFWeaponBase", "m_bLowered");
	M_NETVAR(m_iReloadMode, int, "CTFWeaponBase", "m_iReloadMode");
	M_NETVAR(m_bResetParity, bool, "CTFWeaponBase", "m_bResetParity");
	M_NETVAR(m_bReloadedThroughAnimEvent, bool, "CTFWeaponBase", "m_bReloadedThroughAnimEvent");
	M_NETVAR(m_bDisguiseWeapon, bool, "CTFWeaponBase", "m_bDisguiseWeapon");
	M_NETVAR(m_flLastCritCheckTime, float, "CTFWeaponBase", "m_flLastCritCheckTime");
	M_NETVAR(m_flReloadPriorNextFire, float, "CTFWeaponBase", "m_flReloadPriorNextFire");
	M_NETVAR(m_flLastFireTime, float, "CTFWeaponBase", "m_flLastFireTime");
	M_NETVAR(m_flEffectBarRegenTime, float, "CTFWeaponBase", "m_flEffectBarRegenTime");
	M_NETVAR(m_flObservedCritChance, float, "CTFWeaponBase", "m_flObservedCritChance");
	M_NETVAR(m_flEnergy, float, "CTFWeaponBase", "m_flEnergy");
	M_NETVAR(m_hExtraWearable, EHANDLE, "CTFWeaponBase", "m_hExtraWearable");
	M_NETVAR(m_hExtraWearableViewModel, EHANDLE, "CTFWeaponBase", "m_hExtraWearableViewModel");
	M_NETVAR(m_bBeingRepurposedForTaunt, bool, "CTFWeaponBase", "m_bBeingRepurposedForTaunt");
	M_NETVAR(m_nKillComboClass, int, "CTFWeaponBase", "m_nKillComboClass");
	M_NETVAR(m_nKillComboCount, int, "CTFWeaponBase", "m_nKillComboCount");
	M_NETVAR(m_flInspectAnimEndTime, float, "CTFWeaponBase", "m_flInspectAnimEndTime");
	M_NETVAR(m_nInspectStage, int, "CTFWeaponBase", "m_nInspectStage");
	M_NETVAR(m_iConsecutiveShots, int, "CTFWeaponBase", "m_iConsecutiveShots");

public:
	bool CanFireCiriticalShotHelper(const bool bHeadShot = false);

public:
	inline bool CanHeadShot()
	{
		return ((GetDamageType() & DMG_USE_HITLOCATIONS) && CanFireCiriticalShotHelper(true));
	}
};

#endif //TF_WEAPONBASE_H