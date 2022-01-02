//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================
#ifndef C_TF_PLAYER_H
#define C_TF_PLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_tf_mvm_boss_progress_user.h"
#include "../c_baseplayer.h"
#include "../../shared/econ/ihasattributes.h"
#include "../../shared/econ/econ_item_inventory.h"
#include "../../shared/tf/tf_shareddefs.h"

class C_TFWeaponBase;

enum EBonusEffectFilter_t
{
	kEffectFilter_AttackerOnly,
	kEffectFilter_AttackerTeam,
	kEffectFilter_VictimOnly,
	kEffectFilter_VictimTeam,
	kEffectFilter_AttackerAndVictimOnly,
	kEffectFilter_BothTeams,
};

struct BonusEffect_t
{
	BonusEffect_t(const char* pszSoundName, const char* pszParticleName, EBonusEffectFilter_t eParticleFilter, EBonusEffectFilter_t eSoundFilter, bool bPlaySoundInAttackersEars)
		: m_pszSoundName(pszSoundName)
		, m_pszParticleName(pszParticleName)
		, m_eParticleFilter(eParticleFilter)
		, m_eSoundFilter(eSoundFilter)
		, m_bPlaySoundInAttackersEars(bPlaySoundInAttackersEars)

	{}

	const char* m_pszSoundName;
	const char* m_pszParticleName;
	EBonusEffectFilter_t m_eParticleFilter;
	EBonusEffectFilter_t m_eSoundFilter;
	bool m_bPlaySoundInAttackersEars;
};

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
class C_TFPlayer : public C_BasePlayer, public IHasAttributes, public IInventoryUpdateListener, public C_TFMvMBossProgressUser
{
public:
	virtual					~C_TFPlayer() = 0;

	virtual Vector			GetDeathViewPosition() = 0;
	virtual bool	        IsAllowedToPickUpFlag(void) const = 0;
	virtual void            UpdateMVMEyeGlowEffect(bool bVisible) = 0;

public:
	M_NETVAR(m_bSaveMeParity, bool, "CTFPlayer", "m_bSaveMeParity");
	M_NETVAR(m_bIsMiniBoss, bool, "CTFPlayer", "m_bIsMiniBoss");
	M_NETVAR(m_bIsABot, bool, "CTFPlayer", "m_bIsABot");
	M_NETVAR(m_nBotSkill, int, "CTFPlayer", "m_nBotSkill");
	M_NETVAR(m_hRagdoll, int, "CTFPlayer", "m_hRagdoll");
	M_NETVAR(m_PlayerClass, void*, "CTFPlayer", "m_PlayerClass");
	M_NETVAR(m_iClass, int, "CTFPlayer", "m_iClass");
	M_NETVAR(m_iszClassIcon, const char*, "CTFPlayer", "m_iszClassIcon");
	M_NETVAR(m_iszCustomModel, const char*, "CTFPlayer", "m_iszCustomModel");
	M_NETVAR(m_vecCustomModelOffset, Vector, "CTFPlayer", "m_vecCustomModelOffset");
	M_NETVAR(m_angCustomModelRotation, Vector, "CTFPlayer", "m_angCustomModelRotation");
	M_NETVAR(m_bCustomModelRotates, bool, "CTFPlayer", "m_bCustomModelRotates");
	M_NETVAR(m_bCustomModelRotationSet, bool, "CTFPlayer", "m_bCustomModelRotationSet");
	M_NETVAR(m_bCustomModelVisibleToSelf, bool, "CTFPlayer", "m_bCustomModelVisibleToSelf");
	M_NETVAR(m_bUseClassAnimations, bool, "CTFPlayer", "m_bUseClassAnimations");
	M_NETVAR(m_iClassModelParity, int, "CTFPlayer", "m_iClassModelParity");
	M_NETVAR(m_Shared, void*, "CTFPlayer", "m_Shared");
	M_NETVAR(m_nPlayerCond, int, "CTFPlayer", "m_nPlayerCond");
	M_NETVAR(m_bJumping, bool, "CTFPlayer", "m_bJumping");
	M_NETVAR(m_nNumHealers, int, "CTFPlayer", "m_nNumHealers");
	M_NETVAR(m_iCritMult, int, "CTFPlayer", "m_iCritMult");
	M_NETVAR(m_iAirDash, int, "CTFPlayer", "m_iAirDash");
	M_NETVAR(m_nAirDucked, int, "CTFPlayer", "m_nAirDucked");
	M_NETVAR(m_flDuckTimer, float, "CTFPlayer", "m_flDuckTimer");
	M_NETVAR(m_nPlayerState, int, "CTFPlayer", "m_nPlayerState");
	M_NETVAR(m_iDesiredPlayerClass, int, "CTFPlayer", "m_iDesiredPlayerClass");
	M_NETVAR(m_flMovementStunTime, float, "CTFPlayer", "m_flMovementStunTime");
	M_NETVAR(m_iMovementStunAmount, int, "CTFPlayer", "m_iMovementStunAmount");
	M_NETVAR(m_iMovementStunParity, int, "CTFPlayer", "m_iMovementStunParity");
	M_NETVAR(m_hStunner, EHANDLE, "CTFPlayer", "m_hStunner");
	M_NETVAR(m_iStunFlags, int, "CTFPlayer", "m_iStunFlags");
	M_NETVAR(m_nArenaNumChanges, int, "CTFPlayer", "m_nArenaNumChanges");
	M_NETVAR(m_bArenaFirstBloodBoost, bool, "CTFPlayer", "m_bArenaFirstBloodBoost");
	M_NETVAR(m_iWeaponKnockbackID, int, "CTFPlayer", "m_iWeaponKnockbackID");
	M_NETVAR(m_bLoadoutUnavailable, bool, "CTFPlayer", "m_bLoadoutUnavailable");
	M_NETVAR(m_iItemFindBonus, int, "CTFPlayer", "m_iItemFindBonus");
	M_NETVAR(m_bShieldEquipped, bool, "CTFPlayer", "m_bShieldEquipped");
	M_NETVAR(m_bParachuteEquipped, bool, "CTFPlayer", "m_bParachuteEquipped");
	M_NETVAR(m_iNextMeleeCrit, int, "CTFPlayer", "m_iNextMeleeCrit");
	M_NETVAR(m_iDecapitations, int, "CTFPlayer", "m_iDecapitations");
	M_NETVAR(m_iRevengeCrits, int, "CTFPlayer", "m_iRevengeCrits");
	M_NETVAR(m_iDisguiseBody, int, "CTFPlayer", "m_iDisguiseBody");
	M_NETVAR(m_hCarriedObject, int, "CTFPlayer", "m_hCarriedObject");
	M_NETVAR(m_bCarryingObject, bool, "CTFPlayer", "m_bCarryingObject");
	M_NETVAR(m_flNextNoiseMakerTime, float, "CTFPlayer", "m_flNextNoiseMakerTime");
	M_NETVAR(m_iSpawnRoomTouchCount, int, "CTFPlayer", "m_iSpawnRoomTouchCount");
	M_NETVAR(m_iKillCountSinceLastDeploy, int, "CTFPlayer", "m_iKillCountSinceLastDeploy");
	M_NETVAR(m_flFirstPrimaryAttack, float, "CTFPlayer", "m_flFirstPrimaryAttack");
	M_NETVAR(m_flEnergyDrinkMeter, float, "CTFPlayer", "m_flEnergyDrinkMeter");
	M_NETVAR(m_flHypeMeter, float, "CTFPlayer", "m_flHypeMeter");
	M_NETVAR(m_flChargeMeter, float, "CTFPlayer", "m_flChargeMeter");
	M_NETVAR(m_flInvisChangeCompleteTime, float, "CTFPlayer", "m_flInvisChangeCompleteTime");
	M_NETVAR(m_nDisguiseTeam, int, "CTFPlayer", "m_nDisguiseTeam");
	M_NETVAR(m_nDisguiseClass, int, "CTFPlayer", "m_nDisguiseClass");
	M_NETVAR(m_nDisguiseSkinOverride, int, "CTFPlayer", "m_nDisguiseSkinOverride");
	M_NETVAR(m_nMaskClass, int, "CTFPlayer", "m_nMaskClass");
	M_NETVAR(m_iDisguiseTargetIndex, int, "CTFPlayer", "m_iDisguiseTargetIndex");
	M_NETVAR(m_iDisguiseHealth, int, "CTFPlayer", "m_iDisguiseHealth");
	M_NETVAR(m_bFeignDeathReady, bool, "CTFPlayer", "m_bFeignDeathReady");
	M_NETVAR(m_hDisguiseWeapon, EHANDLE, "CTFPlayer", "m_hDisguiseWeapon");
	M_NETVAR(m_nTeamTeleporterUsed, int, "CTFPlayer", "m_nTeamTeleporterUsed");
	M_NETVAR(m_flCloakMeter, float, "CTFPlayer", "m_flCloakMeter");
	M_NETVAR(m_flSpyTranqBuffDuration, float, "CTFPlayer", "m_flSpyTranqBuffDuration");
	M_NETVAR(m_nDesiredDisguiseTeam, int, "CTFPlayer", "m_nDesiredDisguiseTeam");
	M_NETVAR(m_nDesiredDisguiseClass, int, "CTFPlayer", "m_nDesiredDisguiseClass");
	M_NETVAR(m_flStealthNoAttackExpire, float, "CTFPlayer", "m_flStealthNoAttackExpire");
	M_NETVAR(m_flStealthNextChangeTime, float, "CTFPlayer", "m_flStealthNextChangeTime");
	M_NETVAR(m_bLastDisguisedAsOwnTeam, bool, "CTFPlayer", "m_bLastDisguisedAsOwnTeam");
	M_NETVAR(m_flRageMeter, float, "CTFPlayer", "m_flRageMeter");
	M_NETVAR(m_bRageDraining, bool, "CTFPlayer", "m_bRageDraining");
	M_NETVAR(m_flNextRageEarnTime, float, "CTFPlayer", "m_flNextRageEarnTime");
	M_NETVAR(m_bInUpgradeZone, bool, "CTFPlayer", "m_bInUpgradeZone");
	M_NETVAR(m_flItemChargeMeter, void*, "CTFPlayer", "m_flItemChargeMeter");
	M_NETVAR(m_bPlayerDominated, bool, "CTFPlayer", "m_bPlayerDominated");
	M_NETVAR(m_bPlayerDominatingMe, bool, "CTFPlayer", "m_bPlayerDominatingMe");
	M_NETVAR(m_ScoreData, void*, "CTFPlayer", "m_ScoreData");
	M_NETVAR(m_iCaptures, int, "CTFPlayer", "m_iCaptures");
	M_NETVAR(m_iDefenses, int, "CTFPlayer", "m_iDefenses");
	M_NETVAR(m_iKills, int, "CTFPlayer", "m_iKills");
	M_NETVAR(m_iDeaths, int, "CTFPlayer", "m_iDeaths");
	M_NETVAR(m_iSuicides, int, "CTFPlayer", "m_iSuicides");
	M_NETVAR(m_iDominations, int, "CTFPlayer", "m_iDominations");
	M_NETVAR(m_iRevenge, int, "CTFPlayer", "m_iRevenge");
	M_NETVAR(m_iBuildingsBuilt, int, "CTFPlayer", "m_iBuildingsBuilt");
	M_NETVAR(m_iBuildingsDestroyed, int, "CTFPlayer", "m_iBuildingsDestroyed");
	M_NETVAR(m_iHeadshots, int, "CTFPlayer", "m_iHeadshots");
	M_NETVAR(m_iBackstabs, int, "CTFPlayer", "m_iBackstabs");
	M_NETVAR(m_iHealPoints, int, "CTFPlayer", "m_iHealPoints");
	M_NETVAR(m_iInvulns, int, "CTFPlayer", "m_iInvulns");
	M_NETVAR(m_iTeleports, int, "CTFPlayer", "m_iTeleports");
	M_NETVAR(m_iResupplyPoints, int, "CTFPlayer", "m_iResupplyPoints");
	M_NETVAR(m_iKillAssists, int, "CTFPlayer", "m_iKillAssists");
	M_NETVAR(m_iPoints, int, "CTFPlayer", "m_iPoints");
	M_NETVAR(m_iBonusPoints, int, "CTFPlayer", "m_iBonusPoints");
	M_NETVAR(m_iDamageDone, int, "CTFPlayer", "m_iDamageDone");
	M_NETVAR(m_iCrits, int, "CTFPlayer", "m_iCrits");
	M_NETVAR(m_RoundScoreData, void*, "CTFPlayer", "m_RoundScoreData");
	M_NETVAR(m_ConditionList, void*, "CTFPlayer", "m_ConditionList");
	M_NETVAR(_condition_bits, int, "CTFPlayer", "_condition_bits");
	M_NETVAR(m_iTauntIndex, int, "CTFPlayer", "m_iTauntIndex");
	M_NETVAR(m_iTauntConcept, int, "CTFPlayer", "m_iTauntConcept");
	M_NETVAR(m_nPlayerCondEx, int, "CTFPlayer", "m_nPlayerCondEx");
	M_NETVAR(m_iStunIndex, int, "CTFPlayer", "m_iStunIndex");
	M_NETVAR(m_nHalloweenBombHeadStage, int, "CTFPlayer", "m_nHalloweenBombHeadStage");
	M_NETVAR(m_nPlayerCondEx3, int, "CTFPlayer", "m_nPlayerCondEx3");
	M_NETVAR(m_nPlayerCondEx2, int, "CTFPlayer", "m_nPlayerCondEx2");
	M_NETVAR(m_nStreaks, void*, "CTFPlayer", "m_nStreaks");
	M_NETVAR(m_unTauntSourceItemID_Low, int, "CTFPlayer", "m_unTauntSourceItemID_Low");
	M_NETVAR(m_unTauntSourceItemID_High, int, "CTFPlayer", "m_unTauntSourceItemID_High");
	M_NETVAR(m_flRuneCharge, float, "CTFPlayer", "m_flRuneCharge");
	M_NETVAR(m_bHasPasstimeBall, bool, "CTFPlayer", "m_bHasPasstimeBall");
	M_NETVAR(m_bIsTargetedForPasstimePass, bool, "CTFPlayer", "m_bIsTargetedForPasstimePass");
	M_NETVAR(m_hPasstimePassTarget, EHANDLE, "CTFPlayer", "m_hPasstimePassTarget");
	M_NETVAR(m_askForBallTime, float, "CTFPlayer", "m_askForBallTime");
	M_NETVAR(m_bKingRuneBuffActive, bool, "CTFPlayer", "m_bKingRuneBuffActive");
	M_NETVAR(m_pProvider, int, "CTFPlayer", "m_pProvider");
	M_NETVAR(m_nPlayerCondEx4, int, "CTFPlayer", "m_nPlayerCondEx4");
	M_NETVAR(m_flHolsterAnimTime, float, "CTFPlayer", "m_flHolsterAnimTime");
	M_NETVAR(m_hSwitchTo, EHANDLE, "CTFPlayer", "m_hSwitchTo");
	M_NETVAR(m_hItem, EHANDLE, "CTFPlayer", "m_hItem");
	M_NETVAR(m_angEyeAngles, Vector, "CTFPlayer", "m_angEyeAngles[0]");
	M_NETVAR(m_bIsCoaching, bool, "CTFPlayer", "m_bIsCoaching");
	M_NETVAR(m_hCoach, EHANDLE, "CTFPlayer", "m_hCoach");
	M_NETVAR(m_hStudent, EHANDLE, "CTFPlayer", "m_hStudent");
	M_NETVAR(m_nCurrency, int, "CTFPlayer", "m_nCurrency");
	M_NETVAR(m_nExperienceLevel, int, "CTFPlayer", "m_nExperienceLevel");
	M_NETVAR(m_nExperienceLevelProgress, int, "CTFPlayer", "m_nExperienceLevelProgress");
	M_NETVAR(m_bMatchSafeToLeave, bool, "CTFPlayer", "m_bMatchSafeToLeave");
	M_NETVAR(m_bAllowMoveDuringTaunt, bool, "CTFPlayer", "m_bAllowMoveDuringTaunt");
	M_NETVAR(m_bIsReadyToHighFive, bool, "CTFPlayer", "m_bIsReadyToHighFive");
	M_NETVAR(m_hHighFivePartner, int, "CTFPlayer", "m_hHighFivePartner");
	M_NETVAR(m_nForceTauntCam, int, "CTFPlayer", "m_nForceTauntCam");
	M_NETVAR(m_flTauntYaw, float, "CTFPlayer", "m_flTauntYaw");
	M_NETVAR(m_nActiveTauntSlot, int, "CTFPlayer", "m_nActiveTauntSlot");
	M_NETVAR(m_iTauntItemDefIndex, int, "CTFPlayer", "m_iTauntItemDefIndex");
	M_NETVAR(m_flCurrentTauntMoveSpeed, float, "CTFPlayer", "m_flCurrentTauntMoveSpeed");
	M_NETVAR(m_flVehicleReverseTime, float, "CTFPlayer", "m_flVehicleReverseTime");
	M_NETVAR(m_flMvMLastDamageTime, float, "CTFPlayer", "m_flMvMLastDamageTime");
	M_NETVAR(m_flLastDamageTime, float, "CTFPlayer", "m_flLastDamageTime");
	M_NETVAR(m_bInPowerPlay, bool, "CTFPlayer", "m_bInPowerPlay");
	M_NETVAR(m_iSpawnCounter, int, "CTFPlayer", "m_iSpawnCounter");
	M_NETVAR(m_bArenaSpectator, bool, "CTFPlayer", "m_bArenaSpectator");
	M_NETVAR(m_AttributeManager, void*, "CTFPlayer", "m_AttributeManager");
	M_NETVAR(m_hOuter, EHANDLE, "CTFPlayer", "m_hOuter");
	M_NETVAR(m_ProviderType, int, "CTFPlayer", "m_ProviderType");
	M_NETVAR(m_iReapplyProvisionParity, int, "CTFPlayer", "m_iReapplyProvisionParity");
	M_NETVAR(m_flHeadScale, float, "CTFPlayer", "m_flHeadScale");
	M_NETVAR(m_flTorsoScale, float, "CTFPlayer", "m_flTorsoScale");
	M_NETVAR(m_flHandScale, float, "CTFPlayer", "m_flHandScale");
	M_NETVAR(m_bUseBossHealthBar, bool, "CTFPlayer", "m_bUseBossHealthBar");
	M_NETVAR(m_bUsingVRHeadset, bool, "CTFPlayer", "m_bUsingVRHeadset");
	M_NETVAR(m_bForcedSkin, bool, "CTFPlayer", "m_bForcedSkin");
	M_NETVAR(m_nForcedSkin, int, "CTFPlayer", "m_nForcedSkin");
	M_NETVAR(m_nActiveWpnClip, int, "CTFPlayer", "m_nActiveWpnClip");
	M_NETVAR(m_flKartNextAvailableBoost, float, "CTFPlayer", "m_flKartNextAvailableBoost");
	M_NETVAR(m_iKartHealth, int, "CTFPlayer", "m_iKartHealth");
	M_NETVAR(m_iKartState, int, "CTFPlayer", "m_iKartState");
	M_NETVAR(m_hGrapplingHookTarget, EHANDLE, "CTFPlayer", "m_hGrapplingHookTarget");
	M_NETVAR(m_hSecondaryLastWeapon, EHANDLE, "CTFPlayer", "m_hSecondaryLastWeapon");
	M_NETVAR(m_bUsingActionSlot, bool, "CTFPlayer", "m_bUsingActionSlot");
	M_NETVAR(m_flInspectTime, float, "CTFPlayer", "m_flInspectTime");
	M_NETVAR(m_flHelpmeButtonPressTime, float, "CTFPlayer", "m_flHelpmeButtonPressTime");
	M_NETVAR(m_iCampaignMedals, int, "CTFPlayer", "m_iCampaignMedals");
	M_NETVAR(m_iPlayerSkinOverride, int, "CTFPlayer", "m_iPlayerSkinOverride");
	M_NETVAR(m_bViewingCYOAPDA, bool, "CTFPlayer", "m_bViewingCYOAPDA");
	
public:
	C_TFWeaponBase* GetActiveTFWeapon();

	inline float m_flInvisibility()
	{
		static const int nOffset = GetNetVar("CTFPlayer", "m_flInvisChangeCompleteTime") - 0x8;
		return *reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + nOffset);
	}

	//Credits to KGB
	inline bool InCond(const ETFCond cond)
	{
		const int iCond = static_cast<int>(cond);

		switch (iCond / 32)
		{
		case 0:
		{
			const int bit = (1 << iCond);
			if ((m_nPlayerCond() & bit) == bit)
			{
				return true;
			}

			if ((_condition_bits() & bit) == bit)
			{
				return true;
			}

			break;
		}
		case 1:
		{
			const int bit = 1 << (iCond - 32);
			if ((m_nPlayerCondEx() & bit) == bit)
			{
				return true;
			}

			break;
		}
		case 2:
		{
			const int bit = 1 << (iCond - 64);
			if ((m_nPlayerCondEx2() & bit) == bit)
			{
				return true;
			}

			break;
		}
		case 3:
		{
			const int bit = 1 << (iCond - 96);
			if ((m_nPlayerCondEx3() & bit) == bit)
			{
				return true;
			}

			break;
		}
		case 4:
		{
			const int bit = 1 << (iCond - 128);
			if ((m_nPlayerCondEx4() & bit) == bit)
			{
				return true;
			}

			break;
		}
		default:
			break;
		}

		return false;
	}

	inline bool IsPlayerOnSteamFriendsList()
	{
		return reinterpret_cast<bool(__thiscall*)(void*, void*)>(g_Offsets.m_dwIsPlayerOnSteamFriendsList)(this, this);
	}

	inline bool IsClass(const int nClass)
	{
		return (m_iClass() == nClass);
	}

	inline bool IsCloaked()
	{
		return InCond(TF_COND_STEALTHED)
			|| InCond(TF_COND_STEALTHED_BLINK)
			|| InCond(TF_COND_STEALTHED_USER_BUFF)
			|| InCond(TF_COND_STEALTHED_USER_BUFF_FADING);
	}

	inline bool IsZoomed()
	{
		return InCond(TF_COND_ZOOMED);
	}

	inline bool IsInvulnerable()
	{
		return InCond(TF_COND_INVULNERABLE)
			|| InCond(TF_COND_INVULNERABLE_CARD_EFFECT)
			|| InCond(TF_COND_INVULNERABLE_HIDE_UNLESS_DAMAGE)
			|| InCond(TF_COND_INVULNERABLE_USER_BUFF)
			|| InCond(TF_COND_PHASE);
	}

	inline bool IsCritBoosted()
	{
		return InCond(TF_COND_CRITBOOSTED)
			|| InCond(TF_COND_CRITBOOSTED_BONUS_TIME)
			|| InCond(TF_COND_CRITBOOSTED_CARD_EFFECT)
			|| InCond(TF_COND_CRITBOOSTED_CTF_CAPTURE)
			|| InCond(TF_COND_CRITBOOSTED_FIRST_BLOOD)
			|| InCond(TF_COND_CRITBOOSTED_ON_KILL)
			|| InCond(TF_COND_CRITBOOSTED_PUMPKIN)
			|| InCond(TF_COND_CRITBOOSTED_RAGE_BUFF)
			|| InCond(TF_COND_CRITBOOSTED_RUNE_TEMP)
			|| InCond(TF_COND_CRITBOOSTED_USER_BUFF);
	}

	inline bool IsMiniCritBoosted()
	{
		return InCond(TF_COND_MINICRITBOOSTED_ON_KILL)
			|| InCond(TF_COND_NOHEALINGDAMAGEBUFF)
			|| InCond(TF_COND_ENERGY_BUFF)
			/*|| InCond(TF_COND_CRITBOOSTED_DEMO_CHARGE)*/;
	}

	inline bool IsMarked()
	{
		return InCond(TF_COND_URINE)
			|| InCond(TF_COND_MARKEDFORDEATH)
			|| InCond(TF_COND_MARKEDFORDEATH_SILENT);
	}
};

#endif //C_TF_PLAYER_H