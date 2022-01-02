//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Client-side CBasePlayer.
//
//			- Manages the player's flashlight effect.
//
//=============================================================================//
#ifndef C_BASEPLAYER_H
#define C_BASEPLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_basecombatcharacter.h"
#include "../shared/GameEventListener.h"
#include "../shared/usercmd.h"
#include "../../public/PlayerState.h"
#include "../../public/tier1/utlflags.h"

class CSteamID;
class IRagdoll;
class CViewSetup;
class CHintSystem;
class IGameEvent;
class C_BaseViewModel;

enum PLAYER_ANIM;

struct surfacedata_t;

class C_CommandContext
{
public:
	bool			needsprocessing;

	CUserCmd		cmd;
	int				command_number;
};

class C_PredictionError
{
public:
	float	time;
	Vector	error;
};

#define CHASE_CAM_DISTANCE_MIN	16.0f
#define CHASE_CAM_DISTANCE_MAX	96.0f
#define WALL_OFFSET				6.0f

//-----------------------------------------------------------------------------
// Purpose: Base Player class
//-----------------------------------------------------------------------------
class C_BasePlayer : public C_BaseCombatCharacter, public CGameEventListener
{
public:
	virtual						~C_BasePlayer() = 0;

	virtual void				SharedSpawn() = 0;
	virtual bool				GetSteamID(CSteamID* pID) = 0;
	virtual float				GetPlayerMaxSpeed() = 0;
	virtual void				CalcView(Vector& eyeOrigin, QAngle& eyeAngles, float& zNear, float& zFar, float& fov) = 0;
	virtual void				CalcViewModelView(const Vector& eyeOrigin, const QAngle& eyeAngles) = 0;
	virtual float				CalcRoll(const QAngle& angles, const Vector& velocity, float rollangle, float rollspeed) = 0;
	virtual void				SetPlayerUnderwater(bool state) = 0;
	virtual Vector				Weapon_ShootPosition() = 0;
	virtual void				Weapon_DropPrimary(void) = 0;
	virtual Vector				GetAutoaimVector(float flScale) = 0;
	virtual bool				CreateMove(float flInputSampleTime, CUserCmd* pCmd) = 0;
	virtual void				AvoidPhysicsProps(CUserCmd* pCmd) = 0;
	virtual void				PlayerUse(void) = 0;
	virtual bool				IsUseableEntity(C_BaseEntity* pEntity, unsigned int requiredCaps) = 0;
	virtual int					GetObserverMode() const = 0;
	virtual C_BaseEntity*		GetObserverTarget() const = 0;
	virtual IRagdoll*			GetRepresentativeRagdoll() const = 0;
	virtual void				TeamChange(int iNewTeam) = 0;
	virtual bool				IsAllowedToSwitchWeapons(void) = 0;
	virtual C_BaseCombatWeapon* GetActiveWeaponForSelection(void) = 0;
	virtual C_BaseAnimating*	GetRenderedWeaponModel() = 0;
	virtual bool				IsOverridingViewmodel(void) = 0;
	virtual int					DrawOverriddenViewmodel(C_BaseViewModel* pViewmodel, int flags) = 0;
	virtual float				GetDefaultAnimSpeed(void) = 0;
	virtual void				ThirdPersonSwitch(bool bThirdperson) = 0;
	virtual bool				CanSetSoundMixer(void) = 0;
	virtual int					GetVisionFilterFlags(bool bWeaponsCheck = false) = 0;
	virtual void				CalculateVisionUsingCurrentFlags(void) = 0;
	virtual bool				ViewModel_IsTransparent(void) = 0;
	virtual bool				ViewModel_IsUsingFBTexture(void) = 0;
	virtual void				PreThink(void) = 0;
	virtual void				PostThink(void) = 0;
	virtual void				ItemPreFrame(void) = 0;
	virtual void				ItemPostFrame(void) = 0;
	virtual void				AbortReload(void) = 0;
	virtual void				SelectLastItem(void) = 0;
	virtual void				Weapon_SetLast(C_BaseCombatWeapon* pWeapon) = 0;
	virtual bool				Weapon_ShouldSetLast(C_BaseCombatWeapon* pOldWeapon, C_BaseCombatWeapon* pNewWeapon) = 0;
	virtual bool				Weapon_ShouldSelectItem(C_BaseCombatWeapon* pWeapon) = 0;
	virtual C_BaseCombatWeapon* GetLastWeapon(void) = 0;
	virtual void 				SelectItem(const char* pstr, int iSubType = 0) = 0;
	virtual void				UpdateClientData(void) = 0;
	virtual float				GetFOV(void) = 0;
	virtual bool				IsZooming(void) = 0;
	virtual void				ViewPunch(const QAngle& angleOffset) = 0;
	virtual void				OverrideView(CViewSetup* pSetup) = 0;
	virtual const Vector		GetPlayerMins(void) const = 0;
	virtual const Vector		GetPlayerMaxs(void) const = 0;
	virtual void	            SetVehicleRole(int nRole) = 0;
	virtual void			    SetAnimation(PLAYER_ANIM playerAnim) = 0;
	virtual float				GetMinFOV() const = 0;
	virtual void				PlayPlayerJingle() = 0;
	virtual void				UpdateStepSound(surfacedata_t* psurface, const Vector& vecOrigin, const Vector& vecVelocity) = 0;
	virtual void				PlayStepSound(Vector& vecOrigin, surfacedata_t* psurface, float fvol, bool force) = 0;
	virtual surfacedata_t*		GetFootstepSurface(const Vector& origin, const char* surfaceName) = 0;
	virtual void				GetStepSoundVelocities(float* velwalk, float* velrun) = 0;
	virtual void				SetStepSoundTime(int iStepSoundTime, bool bWalking) = 0; //stepsoundtimes_t
	virtual const char*			GetOverrideStepSound(const char* pszBaseStepSoundName) = 0;
	virtual void				OnEmitFootstepSound(const CSoundParameters& params, const Vector& vecOrigin, float fVolume) = 0;
	virtual void				ExitLadder() = 0;
	virtual CHintSystem*		Hints(void) = 0;
	virtual	IMaterial*          GetHeadLabelMaterial(void) = 0;
	virtual void				OnAchievementAchieved(int iAchievement) = 0;
	virtual void			    UpdateWearables() = 0;
	virtual bool			    CanUseFirstPersonCommand(void) = 0;
	virtual void				CalcObserverView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov) = 0;
	virtual Vector				GetChaseCamViewOffset(CBaseEntity* target) = 0;
	virtual void				CalcInEyeCamView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov) = 0;
	virtual float				GetDeathCamInterpolationTime() = 0;
	virtual void				CalcDeathCamView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov) = 0;
	virtual void				CalcFreezeCamView(Vector& eyeOrigin, QAngle& eyeAngles, float& fov) = 0;
	virtual void				SetLocalViewAngles(const QAngle& viewAngles) = 0;
	virtual void				SetViewAngles(const QAngle& ang) = 0;
	virtual bool				IsDucked(void) const = 0;
	virtual bool				IsDucking(void) const = 0;
	virtual float				GetFallVelocity() = 0;

public:
	M_NETVAR(m_Local, void*, "CBasePlayer", "m_Local");
	M_NETVAR(m_chAreaBits, void*, "CBasePlayer", "m_chAreaBits");
	M_NETVAR(m_chAreaPortalBits, void*, "CBasePlayer", "m_chAreaPortalBits");
	M_NETVAR(m_iHideHUD, int, "CBasePlayer", "m_iHideHUD");
	M_NETVAR(m_flFOVRate, float, "CBasePlayer", "m_flFOVRate");
	M_NETVAR(m_bDucked, bool, "CBasePlayer", "m_bDucked");
	M_NETVAR(m_bDucking, bool, "CBasePlayer", "m_bDucking");
	M_NETVAR(m_bInDuckJump, bool, "CBasePlayer", "m_bInDuckJump");
	M_NETVAR(m_flDucktime, float, "CBasePlayer", "m_flDucktime");
	M_NETVAR(m_flDuckJumpTime, float, "CBasePlayer", "m_flDuckJumpTime");
	M_NETVAR(m_flJumpTime, float, "CBasePlayer", "m_flJumpTime");
	M_NETVAR(m_flFallVelocity, float, "CBasePlayer", "m_flFallVelocity");
	M_NETVAR(m_vecPunchAngle, QAngle, "CBasePlayer", "m_vecPunchAngle");
	M_NETVAR(m_vecPunchAngleVel, Vector, "CBasePlayer", "m_vecPunchAngleVel");
	M_NETVAR(m_bDrawViewmodel, bool, "CBasePlayer", "m_bDrawViewmodel");
	M_NETVAR(m_bWearingSuit, bool, "CBasePlayer", "m_bWearingSuit");
	M_NETVAR(m_bPoisoned, bool, "CBasePlayer", "m_bPoisoned");
	M_NETVAR(m_flStepSize, float, "CBasePlayer", "m_flStepSize");
	M_NETVAR(m_bAllowAutoMovement, bool, "CBasePlayer", "m_bAllowAutoMovement");
	M_NETVAR(m_vecViewOffset, Vector, "CBasePlayer", "m_vecViewOffset[0]");
	M_NETVAR(m_flFriction, float, "CBasePlayer", "m_flFriction");
	M_NETVAR(m_iAmmo, void*, "CBasePlayer", "m_iAmmo");
	M_NETVAR(m_fOnTarget, int, "CBasePlayer", "m_fOnTarget");
	M_NETVAR(m_nTickBase, int, "CBasePlayer", "m_nTickBase");
	M_NETVAR(m_nNextThinkTick, int, "CBasePlayer", "m_nNextThinkTick");
	M_NETVAR(m_hLastWeapon, EHANDLE, "CBasePlayer", "m_hLastWeapon");
	M_NETVAR(m_hGroundEntity, EHANDLE, "CBasePlayer", "m_hGroundEntity");
	M_NETVAR(m_vecVelocity, Vector, "CBasePlayer", "m_vecVelocity[0]");
	M_NETVAR(m_vecBaseVelocity, Vector, "CBasePlayer", "m_vecBaseVelocity");
	M_NETVAR(m_hConstraintEntity, int, "CBasePlayer", "m_hConstraintEntity");
	M_NETVAR(m_vecConstraintCenter, Vector, "CBasePlayer", "m_vecConstraintCenter");
	M_NETVAR(m_flConstraintRadius, float, "CBasePlayer", "m_flConstraintRadius");
	M_NETVAR(m_flConstraintWidth, float, "CBasePlayer", "m_flConstraintWidth");
	M_NETVAR(m_flConstraintSpeedFactor, float, "CBasePlayer", "m_flConstraintSpeedFactor");
	M_NETVAR(m_flDeathTime, float, "CBasePlayer", "m_flDeathTime");
	M_NETVAR(m_nWaterLevel, int, "CBasePlayer", "m_nWaterLevel");
	M_NETVAR(m_flLaggedMovementValue, float, "CBasePlayer", "m_flLaggedMovementValue");
	M_NETVAR(m_AttributeList, void*, "CBasePlayer", "m_AttributeList");
	M_NETVAR(m_iAttributeDefinitionIndex, int, "CBasePlayer", "m_iAttributeDefinitionIndex");
	M_NETVAR(m_flValue, float, "CBasePlayer", "m_flValue");
	M_NETVAR(m_nRefundableCurrency, int, "CBasePlayer", "m_nRefundableCurrency");
	M_NETVAR(pl, CPlayerState, "CBasePlayer", "pl");
	M_NETVAR(deadflag, bool, "CBasePlayer", "deadflag");
	M_NETVAR(m_iFOV, int, "CBasePlayer", "m_iFOV");
	M_NETVAR(m_iFOVStart, int, "CBasePlayer", "m_iFOVStart");
	M_NETVAR(m_flFOVTime, float, "CBasePlayer", "m_flFOVTime");
	M_NETVAR(m_iDefaultFOV, int, "CBasePlayer", "m_iDefaultFOV");
	M_NETVAR(m_hZoomOwner, EHANDLE, "CBasePlayer", "m_hZoomOwner");
	M_NETVAR(m_hVehicle, EHANDLE, "CBasePlayer", "m_hVehicle");
	M_NETVAR(m_hUseEntity, EHANDLE, "CBasePlayer", "m_hUseEntity");
	M_NETVAR(m_iHealth, int, "CBasePlayer", "m_iHealth");
	M_NETVAR(m_lifeState, unsigned char, "CBasePlayer", "m_lifeState");
	M_NETVAR(m_iBonusProgress, int, "CBasePlayer", "m_iBonusProgress");
	M_NETVAR(m_iBonusChallenge, int, "CBasePlayer", "m_iBonusChallenge");
	M_NETVAR(m_flMaxspeed, float, "CBasePlayer", "m_flMaxspeed");
	M_NETVAR(m_fFlags, CUtlFlags<int>, "CBasePlayer", "m_fFlags");
	M_NETVAR(m_iObserverMode, int, "CBasePlayer", "m_iObserverMode");
	M_NETVAR(m_hObserverTarget, EHANDLE, "CBasePlayer", "m_hObserverTarget");
	M_NETVAR(m_hViewModel, EHANDLE, "CBasePlayer", "m_hViewModel[0]");
	M_NETVAR(m_szLastPlaceName, const char*, "CBasePlayer", "m_szLastPlaceName");

public:
	static C_BasePlayer* GetLocalPlayer()
	{
		IClientEntity* pEntity = I::ClientEntityList->GetClientEntity(g_Globals.m_nLocalIndex);

		return pEntity ? pEntity->As<C_BasePlayer*>() : nullptr;
	}

	inline const char* GetPlayerName()
	{
		std::string szRet = "";

		player_info_t pi;
		if (I::EngineClient->GetPlayerInfo(entindex(), &pi))
			szRet += pi.name;

		return szRet.c_str();
	}

	inline int GetUserID()
	{
		player_info_t pi;
		if (I::EngineClient->GetPlayerInfo(entindex(), &pi))
			return pi.userID;

		return -1;
	}
};

#endif // C_BASEPLAYER_H