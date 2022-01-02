//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $NoKeywords: $
//=============================================================================//
#ifndef C_BASEANIMATING_H
#define C_BASEANIMATING_H
#ifdef _WIN32
#pragma once
#endif

#include <array>

#include "c_baseentity.h"
#include "../../public/engine/ivmodelinfo.h"

#define LIPSYNC_POSEPARAM_NAME "mouth"
#define NUM_HITBOX_FIRES 10

/*
class C_BaseClientShader
{
	virtual void RenderMaterial( C_BaseEntity *pEntity, int count, const vec4_t *verts, const vec4_t *normals, const vec2_t *texcoords, vec4_t *lightvalues );
};
*/

class IRagdoll;
class CIKContext;
class CIKState;
class ConVar;
class C_RopeKeyframe;
class CBoneBitList;
class CBoneList;
class KeyValues;
class CJiggleBones;
class IBoneSetup;
class CBoneAccessor;

FORWARD_DECLARE_HANDLE(memhandle_t);

typedef unsigned short MDLHandle_t;

struct ClientModelRenderInfo_t : public ModelRenderInfo_t
{
	// Added space for lighting origin override. Just allocated space, need to set base pointer
	matrix3x4_t lightingOffset;

	// Added space for model to world matrix. Just allocated space, need to set base pointer
	matrix3x4_t modelToWorld;
};

struct RagdollInfo_t
{
	bool		m_bActive;
	float		m_flSaveTime;
	int			m_nNumBones;
	Vector		m_rgBonePos[MAXSTUDIOBONES];
	Quaternion	m_rgBoneQuaternion[MAXSTUDIOBONES];
};

class CAttachmentData
{
public:
	matrix3x4_t	m_AttachmentToWorld;
	QAngle	m_angRotation;
	Vector	m_vOriginVelocity;
	int		m_nLastFramecount : 31;
	int		m_bAnglesComputed : 1;
};

typedef unsigned int ClientSideAnimationListHandle_t;

#define	INVALID_CLIENTSIDEANIMATION_LIST_HANDLE	(ClientSideAnimationListHandle_t)~0

class C_BaseAnimating : public C_BaseEntity, private IModelLoadCallback
{
public:
	virtual                  ~C_BaseAnimating() = 0;

	virtual void			 GetBoneControllers(float controllers[MAXSTUDIOBONECTRLS]) = 0;
	virtual float			 SetBoneController(int iController, float flValue) = 0;
	virtual void             GetPoseParameters(CStudioHdr* pStudioHdr, float poseParameter[MAXSTUDIOPOSEPARAM]) = 0;
	virtual void             BuildTransformations(CStudioHdr* pStudioHdr, Vector* pos, Quaternion q[], const matrix3x4_t& cameraTransform, int boneMask, CBoneBitList& boneComputed) = 0;
	virtual void             ApplyBoneMatrixTransform(matrix3x4_t& transform) = 0;
	virtual void             UpdateIKLocks(float currentTime) = 0;
	virtual void             CalculateIKLocks(float currentTime) = 0;
	virtual int	             InternalDrawModel(int flags) = 0;
	virtual bool             OnInternalDrawModel(ClientModelRenderInfo_t* pInfo) = 0;
	virtual bool             OnPostInternalDrawModel(ClientModelRenderInfo_t* pInfo) = 0;
	virtual IMaterial*       GetEconWeaponMaterialOverride(int iTeam) = 0;
	virtual void	         ControlMouth(CStudioHdr* pStudioHdr) = 0;
	virtual void             DoAnimationEvents(CStudioHdr* pStudio) = 0;
	virtual void             FireEvent(const Vector& origin, const QAngle& angles, int event, const char* options) = 0;
	virtual void             FireObsoleteEvent(const Vector& origin, const QAngle& angles, int event, const char* options) = 0;
	virtual const char*      ModifyEventParticles(const char* token) = 0;
	virtual bool             DispatchMuzzleEffect(const char* options, bool isFirstPerson) = 0;
	virtual	void             StandardBlendingRules(CStudioHdr* pStudioHdr, Vector pos[], Quaternion q[], float currentTime, int boneMask) = 0;
	virtual void             AccumulateLayers(IBoneSetup& boneSetup, Vector pos[], Quaternion q[], float currentTime) = 0;
	virtual void             ChildLayerBlend(Vector pos[], Quaternion q[], float currentTime, int boneMask) = 0;
	virtual void		     AttachEntityToBone(C_BaseAnimating* attachTarget, int boneIndexAttached = -1, Vector bonePosition = Vector(0, 0, 0), QAngle boneAngles = QAngle(0, 0, 0)) = 0;
	virtual void		     NotifyBoneAttached(C_BaseAnimating* attachTarget) = 0;
	virtual void		     UpdateBoneAttachments(void) = 0;
	virtual C_BaseAnimating* BecomeRagdollOnClient() = 0;
	virtual void			 SaveRagdollInfo(int numbones, const matrix3x4_t& cameraTransform, CBoneAccessor* pBoneToWorld) = 0;
	virtual bool			 RetrieveRagdollInfo(Vector* pos, Quaternion* q) = 0;
	virtual bool			 GetRagdollInitBoneArrays(matrix3x4_t* pDeltaBones0, matrix3x4_t* pDeltaBones1, matrix3x4_t* pCurrentBones, float boneDt) = 0;
	virtual	void			 RefreshCollisionBounds(void) = 0;
	virtual void			 SetSequence(int nSequence) = 0;
	virtual void			 StudioFrameAdvance() = 0;
	virtual float			 FrameAdvance(float flInterval = 0.0f) = 0;
	virtual float			 GetSequenceCycleRate(CStudioHdr* pStudioHdr, int iSequence) = 0;
	virtual void			 UpdateClientSideAnimation() = 0;
	virtual unsigned int	 ComputeClientSideAnimationFlags() = 0;
	virtual void             ResetClientsideFrame(void) = 0;
	virtual bool			 IsActivityFinished(void) = 0;
	virtual void			 UncorrectViewModelAttachment(Vector& vOrigin) = 0;
	virtual void             DoMuzzleFlash() = 0;
	virtual void             ProcessMuzzleFlashEvent() = 0;
	virtual void             SetServerIntendedCycle(float intended) = 0;
	virtual float            GetServerIntendedCycle(void) = 0;
	virtual bool			 ShouldResetSequenceOnNewModel(void) = 0;
	virtual bool			 IsViewModel() const = 0;
	virtual void			 FormatViewModelAttachment(int nAttachment, matrix3x4_t& attachmentToWorld) = 0;
	virtual bool		     IsMenuModel() const = 0;
	virtual bool			 CalcAttachments() = 0;
	virtual float			 LastBoneChangedTime() = 0;

public:
	M_NETVAR(m_nSequence, int, "CBaseAnimating", "m_nSequence");
	M_NETVAR(m_nForceBone, int, "CBaseAnimating", "m_nForceBone");
	M_NETVAR(m_vecForce, Vector, "CBaseAnimating", "m_vecForce");
	M_NETVAR(m_nSkin, int, "CBaseAnimating", "m_nSkin");
	M_NETVAR(m_nBody, int, "CBaseAnimating", "m_nBody");
	M_NETVAR(m_nHitboxSet, int, "CBaseAnimating", "m_nHitboxSet");
	M_NETVAR(m_flModelScale, float, "CBaseAnimating", "m_flModelScale");
	M_NETVAR(m_flModelWidthScale, float, "CBaseAnimating", "m_flModelWidthScale");
	M_NETVAR(m_flPlaybackRate, float, "CBaseAnimating", "m_flPlaybackRate");
	M_NETVAR(m_flEncodedController, void*, "CBaseAnimating", "m_flEncodedController");
	M_NETVAR(m_bClientSideAnimation, bool, "CBaseAnimating", "m_bClientSideAnimation");
	M_NETVAR(m_bClientSideFrameReset, bool, "CBaseAnimating", "m_bClientSideFrameReset");
	M_NETVAR(m_nNewSequenceParity, int, "CBaseAnimating", "m_nNewSequenceParity");
	M_NETVAR(m_nResetEventsParity, int, "CBaseAnimating", "m_nResetEventsParity");
	M_NETVAR(m_nMuzzleFlashParity, int, "CBaseAnimating", "m_nMuzzleFlashParity");
	M_NETVAR(m_hLightingOrigin, EHANDLE, "CBaseAnimating", "m_hLightingOrigin");
	M_NETVAR(m_hLightingOriginRelative, EHANDLE, "CBaseAnimating", "m_hLightingOriginRelative");
	M_NETVAR(m_flCycle, float, "CBaseAnimating", "m_flCycle");
	M_NETVAR(m_fadeMinDist, float, "CBaseAnimating", "m_fadeMinDist");
	M_NETVAR(m_fadeMaxDist, float, "CBaseAnimating", "m_fadeMaxDist");
	M_NETVAR(m_flFadeScale, float, "CBaseAnimating", "m_flFadeScale");

public:
	inline std::array<float, MAXSTUDIOPOSEPARAM>& m_flPoseParameter()
	{
		static const int nOffset = GetNetVar("CBaseAnimating", "m_flPoseParameter");
		return *reinterpret_cast<std::array<float, MAXSTUDIOPOSEPARAM>*>(reinterpret_cast<DWORD>(this) + nOffset);
	}

	inline CStudioHdr* &m_pStudioHdr() 
	{
		static const int nOffset = (GetNetVar("CBaseAnimating", "m_nMuzzleFlashParity") + 9);
		return *reinterpret_cast<CStudioHdr**>(reinterpret_cast<DWORD>(this) + nOffset);
	}
	
	inline MDLHandle_t& m_hStudioHdr()
	{
		static const int nOffset = (GetNetVar("CBaseAnimating", "m_nMuzzleFlashParity") + 13);
		return *reinterpret_cast<MDLHandle_t*>(reinterpret_cast<DWORD>(this) + nOffset);
	}

public:
	inline bool GetHitboxPosition(const int nHitbox, Vector& vPosition)
	{
		const model_t* pModel = this->GetModel();

		if (!pModel)
			return false;

		const studiohdr_t* pStudioHdr = I::ModelInfoClient->GetStudiomodel(pModel);

		if (!pStudioHdr)
			return false;

		const mstudiobbox_t* pBox = pStudioHdr->pHitbox(nHitbox, this->m_nHitboxSet());

		if (!pBox || (pBox->bone >= MAXSTUDIOBONES) || (pBox->bone < 0))
			return false;

		matrix3x4_t BoneMatrix[MAXSTUDIOBONES];
		if (!SetupBones(BoneMatrix, MAXSTUDIOBONES, BONE_USED_BY_HITBOX, I::GlobalVars->curtime))
			return false;

		VectorTransform((pBox->bbmax + pBox->bbmin) * 0.5f, BoneMatrix[pBox->bone], vPosition);
		return true;
	}
};

#endif //C_BASEANIMATING_H