//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: A base class for the client-side representation of entities.
//
//			This class encompasses both entities that are created on the server
//			and networked to the client AND entities that are created on the
//			client.
//
// $NoKeywords: $
//===========================================================================//
#ifndef C_BASEENTITY_H
#define C_BASEENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "../../public/mathlib/vector.h"
#include "../../public/icliententity.h"
#include "../../public/string_t.h"
#include "../../public/bspflags.h"
#include "../../public/engine/ICollideable.h"
#include "../../public/datamap.h"
#include "../shared/usercmd.h"
#include "../shared/ehandle.h"

namespace I { inline int32* PredictionRandomSeed = nullptr; }

class IInterpolatedVar;
class CDmgAccumulator;
class CEntityMapData;
class C_BaseAnimating;
class ISave;
class IRestore;
class IPhysicsObject;
class C_Team;
class IClientVehicle;
class CStudioHdr;
class CNewParticleEffect;
class CDamageModifier;
class C_BaseCombatWeapon;
class ITraceFilter;
class CTakeDamageInfo;
class C_BaseCombatCharacter;
class C_BaseCombatWeapon;
class C_BaseEntity;

struct FireBulletsInfo_t;
struct EmitSound_t;

typedef unsigned int AimEntsListHandle_t;

#define INVALID_AIMENTS_LIST_HANDLE	(AimEntsListHandle_t)~0

enum CollideType_t
{
	ENTITY_SHOULD_NOT_COLLIDE = 0,
	ENTITY_SHOULD_COLLIDE,
	ENTITY_SHOULD_RESPOND
};

class VarMapEntry_t
{
public:
	unsigned short		type;
	unsigned short		m_bNeedsToInterpolate;	// Set to false when this var doesn't
												// need Interpolate() called on it anymore.
	void* data;
	IInterpolatedVar* watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	CUtlVector<VarMapEntry_t>	m_Entries;
	int							m_nInterpolatedEntries;
	float						m_lastInterpolationTime;
};

//How many data slots to use when in multiplayer.
#define MULTIPLAYER_BACKUP 90

struct serialentity_t;

typedef CHandle<C_BaseEntity> EHANDLE; // The client's version of EHANDLE.

typedef void (C_BaseEntity::* BASEPTR)(void);
typedef void (C_BaseEntity::* ENTITYFUNCPTR)(C_BaseEntity* pOther);

// For entity creation on the client
typedef C_BaseEntity* (*DISPATCHFUNCTION)(void);

#if !defined( NO_ENTITY_PREDICTION )
//-----------------------------------------------------------------------------
// Purpose: For fully client side entities we use this information to determine
//  authoritatively if the server has acknowledged creating this entity, etc.
//-----------------------------------------------------------------------------
struct PredictionContext
{
	PredictionContext()
	{
		m_bActive = false;
		m_nCreationCommandNumber = -1;
		m_pszCreationModule = NULL;
		m_nCreationLineNumber = 0;
		m_hServerEntity = NULL;
	}

	//The command_number of the usercmd which created this entity
	bool						m_bActive;
	int							m_nCreationCommandNumber;
	char const*                 m_pszCreationModule;
	int							m_nCreationLineNumber;

	//The entity to whom we are attached
	CHandle<C_BaseEntity>		m_hServerEntity;
};
#endif

//-----------------------------------------------------------------------------
// Purpose: think contexts
//-----------------------------------------------------------------------------
struct thinkfunc_t
{
	BASEPTR		m_pfnThink;
	string_t	m_iszContext;
	int			m_nNextThinkTick;
	int			m_nLastThinkTick;
};

// Entity flags that only exist on the client.
#define ENTCLIENTFLAG_GETTINGSHADOWRENDERBOUNDS	0x0001		// Tells us if we're getting the real ent render bounds or the shadow render bounds.
#define ENTCLIENTFLAG_DONTUSEIK					0x0002		// Don't use IK on this entity even if its model has IK.
#define ENTCLIENTFLAG_ALWAYS_INTERPOLATE		0x0004		// Used by view models.

//-----------------------------------------------------------------------------
// Purpose: Base client side entity object
//-----------------------------------------------------------------------------
class C_BaseEntity : public IClientEntity
{
public:
	virtual                         ~C_BaseEntity() = 0;

	virtual datamap_t*              GetDataDescMap() = 0;
	virtual void*                   YouForgotToImplementOrDeclareClientClass() = 0;
	virtual datamap_t*              GetPredDescMap() = 0;

	virtual void					FireBullets(const FireBulletsInfo_t& info) = 0;
	virtual void					ModifyFireBulletsDamage(CTakeDamageInfo* dmgInfo) = 0;
	virtual bool					ShouldDrawUnderwaterBulletBubbles() = 0;
	virtual bool					ShouldDrawWaterImpacts(void) = 0;
	virtual bool					HandleShotImpactingWater(const FireBulletsInfo_t& info, const Vector& vecEnd, ITraceFilter* pTraceFilter, Vector* pVecTracerDest) = 0;
	virtual ITraceFilter*           GetBeamTraceFilter(void) = 0;
	virtual void					DispatchTraceAttack(const CTakeDamageInfo* info, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator = NULL) = 0;
	virtual void					TraceAttack(const CTakeDamageInfo* info, const Vector& vecDir, trace_t* ptr, CDmgAccumulator* pAccumulator = NULL) = 0;
	virtual void					DoImpactEffect(trace_t& tr, int nDamageType) = 0;
	virtual void					MakeTracer(const Vector& vecTracerSrc, const trace_t& tr, int iTracerType) = 0;
	virtual int						GetTracerAttachment(void) = 0;
	virtual int						BloodColor() = 0;
	virtual const char*             GetTracerType() = 0;
	virtual void					Spawn(void) = 0;
	virtual void					SpawnClientEntity(void) = 0;
	virtual void					Precache(void) = 0;
	virtual void					Activate() = 0;
	virtual void					ParseMapData(CEntityMapData* mapData) = 0;
	virtual bool					KeyValue(const char* szKeyName, const char* szValue) = 0;
	virtual bool					KeyValue(const char* szKeyName, float flValue) = 0;
	virtual bool					KeyValue(const char* szKeyName, const Vector& vecValue) = 0;
	virtual bool					GetKeyValue(const char* szKeyName, char* szValue, int iMaxLen) = 0;
	virtual bool					Init(int entnum, int iSerialNum) = 0;
	virtual C_BaseAnimating*        GetBaseAnimating() = 0;
	virtual void					SetClassname(const char* className) = 0;
	virtual void			        RecordToolMessage() = 0;
	virtual Vector					GetObserverCamOrigin(void) = 0;
	virtual bool					TestCollision(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual bool					TestHitboxes(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual float					GetAttackDamageScale(void) = 0;
	virtual void					ValidateModelIndex(void) = 0;
	virtual void					SetDormant(bool bDormant) = 0;
	virtual int				        GetEFlags() const = 0;
	virtual void			        SetEFlags(int iEFlags) = 0;
	virtual bool					ShouldSavePhysics() = 0;
	virtual void					OnSave() = 0;
	virtual void					OnRestore() = 0;
	virtual int						ObjectCaps(void) = 0;
	virtual int						Save(ISave& save) = 0;
	virtual int						Restore(IRestore& restore) = 0;
	virtual bool	                CreateVPhysics() = 0;
	virtual void	                VPhysicsDestroyObject(void) = 0;
	virtual void					VPhysicsUpdate(IPhysicsObject* pPhysics) = 0;
	virtual int						VPhysicsGetObjectList(IPhysicsObject** pList, int listMax) = 0;
	virtual bool					VPhysicsIsFlesh(void) = 0;
	virtual void					AddEntity(void) = 0;
	virtual const Vector&           GetPrevLocalOrigin() const = 0;
	virtual const QAngle&           GetPrevLocalAngles() const = 0;
	virtual int						CalcOverrideModelIndex() = 0;
	virtual const Vector&           WorldAlignMins() const = 0;
	virtual const Vector&           WorldAlignMaxs() const = 0;
	virtual const Vector&           WorldSpaceCenter() const = 0;
	virtual void					ComputeWorldSpaceSurroundingBox(Vector* pVecWorldMins, Vector* pVecWorldMaxs) = 0;
	virtual float					GetHealthBarHeightOffset() const = 0;
	virtual SolidType_t				GetSolid(void) const = 0;
	virtual int			 			GetSolidFlags(void) const = 0;
	virtual bool					GetAttachment(int number, Vector& origin) = 0;
	virtual bool					GetAttachmentVelocity(int number, Vector& originVel, Quaternion& angleVel) = 0;
	virtual C_Team*                 GetTeam(void) const = 0;
	virtual int						GetTeamNumber(void) const = 0;
	virtual void					ChangeTeam(int iTeamNum) = 0;
	virtual int						GetRenderTeamNumber(void) = 0;
	virtual bool					InSameTeam(const C_BaseEntity* pEntity) const = 0;
	virtual bool					InLocalTeam(void) = 0;
	virtual bool					IsValidIDTarget(void) = 0;
	virtual const char*             GetIDString(void) = 0;
	virtual void                    ModifyEmitSoundParams(EmitSound_t& params) = 0;
	virtual bool                    InitializeAsClientEntity(const char* pszModelName, int renderGroup) = 0; //RenderGroup_t
	virtual void					Simulate() = 0;
	virtual IClientVehicle*         GetClientVehicle() = 0;
	virtual void					GetAimEntOrigin(IClientEntity* pAttachedTo, Vector* pAbsOrigin, QAngle* pAbsAngles) = 0;
	virtual const Vector&           GetOldOrigin() = 0;
	virtual int			            GetRenderGroup() = 0; //RenderGroup_t
	virtual void					GetToolRecordingState(KeyValues* msg) = 0;
	virtual void					CleanupToolRecordingState(KeyValues* msg) = 0;
	virtual CollideType_t			GetCollideType(void) = 0;
	virtual void					UpdateVisibility() = 0;
	virtual bool					IsSelfAnimating() = 0;
	virtual	void					OnLatchInterpolatedVariables(int flags) = 0;
	virtual CStudioHdr*             OnNewModel() = 0;
	virtual void					OnNewParticleEffect(const char* pszParticleName, CNewParticleEffect* pNewParticleEffect) = 0;
	virtual void					ResetLatched() = 0;
	virtual bool					Interpolate(float currentTime) = 0;
	virtual bool					IsSubModel(void) = 0;
	virtual void					CreateLightEffects(void) = 0;
	virtual void					Clear(void) = 0;
	virtual int						DrawBrushModel(bool bTranslucent, int nFlags, bool bTwoPass) = 0;
	virtual float					GetTextureAnimationStartTime() = 0;
	virtual void					TextureAnimationWrapped() = 0;
	virtual void					SetNextClientThink(float nextThinkTime) = 0;
	virtual void					SetHealth(int iHealth) = 0;
	virtual int						GetHealth() const = 0;
	virtual int						GetMaxHealth() const = 0;
	virtual bool					IsVisibleToTargetID(void) const = 0;
	virtual bool					IsHealthBarVisible(void) const = 0;
	virtual ShadowType_t			ShadowCastType() = 0;
	virtual bool					ShouldReceiveProjectedTextures(int flags) = 0;
	virtual void					AddDecal(const Vector& rayStart, const Vector& rayEnd, const Vector& decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t& tr, int maxLODToDecal = ADDDECAL_TO_ALL_LODS) = 0;
	virtual void					AddColoredDecal(const Vector& rayStart, const Vector& rayEnd, const Vector& decalCenter, int hitbox, int decalIndex, bool doTrace, trace_t& tr, Color cColor, int maxLODToDecal = ADDDECAL_TO_ALL_LODS) = 0;
	virtual bool					IsClientCreated(void) const = 0;
	virtual void					UpdateOnRemove(void) = 0;
	virtual void					SUB_Remove(void) = 0;
	virtual void					SetPredictable(bool state) = 0;
	virtual int						RestoreData(const char* context, int slot, int type) = 0;
	virtual char const*             DamageDecal(int bitsDamageType, int gameMaterial) = 0;
	virtual void					DecalTrace(trace_t* pTrace, char const* decalName) = 0;
	virtual void					ImpactTrace(trace_t* pTrace, int iDamageType, const char* pCustomImpactName) = 0;
	virtual bool					ShouldPredict(void) = 0;
	virtual void					Think(void) = 0;
	virtual bool			        CanBePoweredUp(void) = 0;
	virtual bool			        AttemptToPowerup(int iPowerup, float flTime, float flAmount = 0, C_BaseEntity* pAttacker = NULL, CDamageModifier* pDamageModifier = NULL) = 0;
	virtual bool			        IsCurrentlyTouching(void) const = 0;
	virtual void			        StartTouch(C_BaseEntity* pOther) = 0;
	virtual void			        Touch(C_BaseEntity* pOther) = 0;
	virtual void			        EndTouch(C_BaseEntity* pOther) = 0;
	virtual unsigned int	        PhysicsSolidMaskForEntity(void) const = 0;
	virtual void					PhysicsSimulate(void) = 0;
	virtual bool					IsAlive(void) = 0;
	virtual bool					IsPlayer(void) const = 0;
	virtual bool					IsBaseCombatCharacter(void) = 0;
	virtual C_BaseCombatCharacter*  MyCombatCharacterPointer(void) = 0;
	virtual bool					IsNPC(void) = 0;
	virtual bool					IsNextBot() = 0;
	virtual bool					IsBaseObject(void) const = 0;
	virtual bool					IsBaseCombatWeapon(void) const = 0;
	virtual C_BaseCombatWeapon*     MyCombatWeaponPointer() = 0;
	virtual bool					IsCombatItem(void) const = 0;
	virtual bool					IsBaseTrain(void) const = 0;
	virtual Vector			        EyePosition(void) = 0;
	virtual const QAngle&           EyeAngles(void) = 0;
	virtual const QAngle&           LocalEyeAngles(void) = 0;
	virtual Vector		            EarPosition(void) = 0;
	virtual bool		            ShouldCollide(int collisionGroup, int contentsMask) const = 0;
	virtual const Vector&           GetViewOffset() const = 0;
	virtual void		            SetViewOffset(const Vector& v) = 0;
	virtual void                    ClientAdjustStartSoundParams(void* unk) = 0; //EmitSound_t
	virtual void                    ClientAdjustStartSoundParams2(void* unk) = 0; //StartSoundParams_t
	virtual bool                    ShouldInterpolate() = 0;
	virtual unsigned char           GetClientSideFade() = 0;
	virtual void                    BoneMergeFastCullBloat(Vector& localMins, Vector& localMaxs, const Vector& thisEntityMins, const Vector& thisEntityMaxs) const = 0;
	virtual bool					OnPredictedEntityRemove(bool isbeingremoved, C_BaseEntity* predicted) = 0;
	virtual C_BaseEntity*           GetShadowUseOtherEntity(void) const = 0;
	virtual void					SetShadowUseOtherEntity(C_BaseEntity* pEntity) = 0;
	virtual bool					AddRagdollToFadeQueue(void) = 0;
	virtual bool					IsDeflectable() = 0;
	virtual int						GetStudioBody(void) = 0;
	virtual void                    PerformCustomPhysics(Vector* pNewPosition, Vector* pNewVelocity, QAngle* pNewAngles, QAngle* pNewAngVelocity) = 0;
	virtual bool                    ValidateEntityAttachedToPlayer(bool& bShouldRetry) = 0;

public:
	enum {
		INTERPOLATE_STOP = 0,
		INTERPOLATE_CONTINUE
	};

	enum thinkmethods_t {
		THINK_FIRE_ALL_FUNCTIONS,
		THINK_FIRE_BASE_ONLY,
		THINK_FIRE_ALL_BUT_BASE
	};

public:
	M_NETVAR(m_flAnimTime, int, "CBaseEntity", "m_flAnimTime");
	M_NETVAR(m_flSimulationTime, float, "CBaseEntity", "m_flSimulationTime");
	M_NETVAR(m_ubInterpolationFrame, int, "CBaseEntity", "m_ubInterpolationFrame");
	M_NETVAR(m_vecOrigin, Vector, "CBaseEntity", "m_vecOrigin");
	M_NETVAR(m_angRotation, Vector, "CBaseEntity", "m_angRotation");
	M_NETVAR(m_nModelIndex, int, "CBaseEntity", "m_nModelIndex");
	M_NETVAR(m_fEffects, int, "CBaseEntity", "m_fEffects");
	M_NETVAR(m_nRenderMode, int, "CBaseEntity", "m_nRenderMode");
	M_NETVAR(m_nRenderFX, int, "CBaseEntity", "m_nRenderFX");
	M_NETVAR(m_clrRender, int, "CBaseEntity", "m_clrRender");
	M_NETVAR(m_iTeamNum, int, "CBaseEntity", "m_iTeamNum");
	M_NETVAR(m_CollisionGroup, int, "CBaseEntity", "m_CollisionGroup");
	M_NETVAR(m_flElasticity, float, "CBaseEntity", "m_flElasticity");
	M_NETVAR(m_flShadowCastDistance, float, "CBaseEntity", "m_flShadowCastDistance");
	M_NETVAR(m_hOwnerEntity, EHANDLE, "CBaseEntity", "m_hOwnerEntity");
	M_NETVAR(m_hEffectEntity, EHANDLE, "CBaseEntity", "m_hEffectEntity");
	M_NETVAR(moveparent, EHANDLE, "CBaseEntity", "moveparent");
	M_NETVAR(m_iParentAttachment, int, "CBaseEntity", "m_iParentAttachment");
	M_NETVAR(m_Collision, ICollideable*, "CBaseEntity", "m_Collision");
	M_NETVAR(m_vecMinsPreScaled, Vector, "CBaseEntity", "m_vecMinsPreScaled");
	M_NETVAR(m_vecMaxsPreScaled, Vector, "CBaseEntity", "m_vecMaxsPreScaled");
	M_NETVAR(m_vecMins, Vector, "CBaseEntity", "m_vecMins");
	M_NETVAR(m_vecMaxs, Vector, "CBaseEntity", "m_vecMaxs");
	M_NETVAR(m_nSolidType, int, "CBaseEntity", "m_nSolidType");
	M_NETVAR(m_usSolidFlags, unsigned short, "CBaseEntity", "m_usSolidFlags");
	M_NETVAR(m_nSurroundType, int, "CBaseEntity", "m_nSurroundType");
	M_NETVAR(m_triggerBloat, int, "CBaseEntity", "m_triggerBloat");
	M_NETVAR(m_bUniformTriggerBloat, bool, "CBaseEntity", "m_bUniformTriggerBloat");
	M_NETVAR(m_vecSpecifiedSurroundingMinsPreScaled, Vector, "CBaseEntity", "m_vecSpecifiedSurroundingMinsPreScaled");
	M_NETVAR(m_vecSpecifiedSurroundingMaxsPreScaled, Vector, "CBaseEntity", "m_vecSpecifiedSurroundingMaxsPreScaled");
	M_NETVAR(m_vecSpecifiedSurroundingMins, Vector, "CBaseEntity", "m_vecSpecifiedSurroundingMins");
	M_NETVAR(m_vecSpecifiedSurroundingMaxs, Vector, "CBaseEntity", "m_vecSpecifiedSurroundingMaxs");
	M_NETVAR(m_iTextureFrameIndex, int, "CBaseEntity", "m_iTextureFrameIndex");
	M_NETVAR(m_PredictableID, int, "CBaseEntity", "m_PredictableID");
	M_NETVAR(m_bIsPlayerSimulated, bool, "CBaseEntity", "m_bIsPlayerSimulated");
	M_NETVAR(m_bSimulatedEveryTick, bool, "CBaseEntity", "m_bSimulatedEveryTick");
	M_NETVAR(m_bAnimatedEveryTick, bool, "CBaseEntity", "m_bAnimatedEveryTick");
	M_NETVAR(m_bAlternateSorting, bool, "CBaseEntity", "m_bAlternateSorting");
	M_NETVAR(m_nModelIndexOverrides, void*, "CBaseEntity", "m_nModelIndexOverrides");

public:
	inline int GetMoveType()
	{
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD>(this) + 0x1A4);
	}

	inline C_BaseEntity* FirstMoveChild()
	{
		static const int nOffset = (GetNetVar("CBaseEntity", "moveparent") - 0xC);
		return I::ClientEntityList->GetClientEntityFromHandle(*reinterpret_cast<unsigned long*>(reinterpret_cast<DWORD>(this) + nOffset))->As<C_BaseEntity*>();
	}

	inline C_BaseEntity* NextMovePeer()
	{
		static const int nOffset = (GetNetVar("CBaseEntity", "moveparent") - 0x8);
		return I::ClientEntityList->GetClientEntityFromHandle(*reinterpret_cast<unsigned long*>(reinterpret_cast<DWORD>(this) + nOffset))->As<C_BaseEntity*>();
	}

	inline float& m_flOldSimulationTime()
	{
		static const int nOffset = GetNetVar("CBaseEntity", "m_flSimulationTime") + 0x4;
		return *reinterpret_cast<float*>(reinterpret_cast<DWORD>(this) + nOffset);
	}

	inline CUserCmd*& m_pCurrentCommand()
	{
		static const int nOffset = (GetNetVar("CBasePlayer", "m_hConstraintEntity") - 0x04);

		return *reinterpret_cast<CUserCmd**>(reinterpret_cast<DWORD>(this) + nOffset);
	}

	inline bool PhysicsRunThink(const thinkmethods_t thinkMethod = THINK_FIRE_ALL_FUNCTIONS)
	{
		return reinterpret_cast<bool(__thiscall*)(void*, thinkmethods_t)>(g_Offsets.m_dwPhysicsRunThink)(this, thinkMethod);
	}

	inline int SaveData(const char* const context, const int slot, const int type)
	{
		return reinterpret_cast<int(__thiscall*)(void*, const char*, int, int)>(g_Offsets.m_dwSaveData)(this, context, slot, type);
	}

	static void SetPredictionRandomSeed(const int nSeed)
	{
		*I::PredictionRandomSeed = nSeed;
	}

public:
	inline C_BaseEntity* GetRootMoveParent()
	{
		C_BaseEntity* pEntity = this;
		C_BaseEntity* pParent = this->moveparent();

		while (pParent)
		{
			pEntity = pParent;
			pParent = pEntity->moveparent();
		}

		return pEntity;
	}
};

#endif //C_BASEENTITY_H