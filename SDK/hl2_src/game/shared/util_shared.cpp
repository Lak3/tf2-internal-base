//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
//=============================================================================//
#include "util_shared.h"

#include "../client/tf/c_tf_player.h"
#include "../../public/mathlib/mathlib.h"
#include "../../public/model_types.h"
#include "../../public/mathlib/IceKey.h"
#include "../../public/tier1/checksum_crc.h"
#include "../../public/engine/ivmodelinfo.h"

#include <time.h>

//#ifdef TF_CLIENT_DLL
//#include "cdll_util.h"
//#endif

float UTIL_VecToYaw(const Vector& vec)
{
	if (vec.y == 0 && vec.x == 0)
		return 0;

	float yaw = atan2f(vec.y, vec.x);

	yaw = RAD2DEG(yaw);

	if (yaw < 0)
		yaw += 360;

	return yaw;
}

float UTIL_VecToPitch(const Vector& vec)
{
	if (vec.y == 0.0f && vec.x == 0.0f)
	{
		if (vec.z < 0)
			return 180.0f;
		else
			return -180.0f;
	}

	float dist = vec.Length2D();
	float pitch = atan2f(-vec.z, dist);

	pitch = RAD2DEG(pitch);

	return pitch;
}

float UTIL_VecToYaw(const matrix3x4_t& matrix, const Vector& vec)
{
	Vector tmp = vec;
	VectorNormalize(tmp);

	const float x = matrix[0][0] * tmp.x + matrix[1][0] * tmp.y + matrix[2][0] * tmp.z;
	const float y = matrix[0][1] * tmp.x + matrix[1][1] * tmp.y + matrix[2][1] * tmp.z;

	if (x == 0.0f && y == 0.0f)
		return 0.0f;

	float yaw = atan2f(-y, x);

	yaw = RAD2DEG(yaw);

	if (yaw < 0)
		yaw += 360.0f;

	return yaw;
}

float UTIL_VecToPitch(const matrix3x4_t& matrix, const Vector& vec)
{
	Vector tmp = vec;
	VectorNormalize(tmp);

	const float x = matrix[0][0] * tmp.x + matrix[1][0] * tmp.y + matrix[2][0] * tmp.z;
	const float z = matrix[0][2] * tmp.x + matrix[1][2] * tmp.y + matrix[2][2] * tmp.z;

	if (x == 0.0f && z == 0.0f)
		return 0.0f;

	float pitch = atan2f(z, x);

	pitch = RAD2DEG(pitch);

	if (pitch < 0)
		pitch += 360.0f;

	return pitch;
}

Vector UTIL_YawToVector(float yaw)
{
	Vector ret;

	ret.z = 0.0f;
	float angle = DEG2RAD(yaw);
	FastSinCos(angle, &ret.y, &ret.x);

	return ret;
}

//-----------------------------------------------------------------------------
// Purpose: Helper function get get determinisitc random values for shared/prediction code
// Input  : seedvalue - 
//			*module - 
//			line - 
// Output : static int
//-----------------------------------------------------------------------------
static int SeedFileLineHash(int seedvalue, const char* sharedname, int additionalSeed)
{
	CRC32_t retval;

	CRC32_Init(&retval);

	CRC32_ProcessBuffer(&retval, (void*)&seedvalue, sizeof(int));
	CRC32_ProcessBuffer(&retval, (void*)&additionalSeed, sizeof(int));
	CRC32_ProcessBuffer(&retval, (void*)sharedname, Q_strlen(sharedname));

	CRC32_Final(&retval);

	return (int)(retval);
}

/*
float SharedRandomFloat(const char* sharedname, float flMinVal, float flMaxVal, int additionalSeed)
{
	//assert(C_BaseEntity::GetPredictionRandomSeed() != -1);

	int seed = SeedFileLineHash(C_BaseEntity::GetPredictionRandomSeed(), sharedname, additionalSeed);
	RandomSeed(seed);
	return RandomFloat(flMinVal, flMaxVal);
}

int SharedRandomInt(const char* sharedname, int iMinVal, int iMaxVal, int additionalSeed)
{
	assert(C_BaseEntity::GetPredictionRandomSeed() != -1);

	int seed = SeedFileLineHash(C_BaseEntity::GetPredictionRandomSeed(), sharedname, additionalSeed);
	RandomSeed(seed);
	return RandomInt(iMinVal, iMaxVal);
}

Vector SharedRandomVector(const char* sharedname, float minVal, float maxVal, int additionalSeed)
{
	assert(C_BaseEntity::GetPredictionRandomSeed() != -1);

	int seed = SeedFileLineHash(C_BaseEntity::GetPredictionRandomSeed(), sharedname, additionalSeed);
	RandomSeed(seed);
	// HACK:  Can't call RandomVector/Angle because it uses rand() not vstlib Random*() functions!
	// Get a random vector.
	Vector vRandom;
	vRandom.x = RandomFloat(minVal, maxVal);
	vRandom.y = RandomFloat(minVal, maxVal);
	vRandom.z = RandomFloat(minVal, maxVal);
	return vRandom;
}

QAngle SharedRandomAngle(const char* sharedname, float minVal, float maxVal, int additionalSeed)
{
	assert(C_BaseEntity::GetPredictionRandomSeed() != -1);

	int seed = SeedFileLineHash(C_BaseEntity::GetPredictionRandomSeed(), sharedname, additionalSeed);
	RandomSeed(seed);

	// HACK:  Can't call RandomVector/Angle because it uses rand() not vstlib Random*() functions!
	// Get a random vector.
	Vector vRandom;
	vRandom.x = RandomFloat(minVal, maxVal);
	vRandom.y = RandomFloat(minVal, maxVal);
	vRandom.z = RandomFloat(minVal, maxVal);
	return QAngle(vRandom.x, vRandom.y, vRandom.z);
}
*/

//-----------------------------------------------------------------------------
//
// Shared client/server trace filter code
//
//-----------------------------------------------------------------------------
bool PassServerEntityFilter(const IHandleEntity* pTouch, const IHandleEntity* pPass)
{
	if (!pPass)
		return true;

	if (pTouch == pPass)
		return false;

	const C_BaseEntity* pEntTouch = EntityFromEntityHandle(pTouch);
	const C_BaseEntity* pEntPass = EntityFromEntityHandle(pPass);
	if (!pEntTouch || !pEntPass)
		return true;

	// don't clip against own missiles
	//if (pEntTouch->m_hOwnerEntity() == pEntPass)
	//	return false;

	// don't clip against owner
	//if (pEntPass->m_hOwnerEntity() == pEntTouch)
	//	return false;


	return true;
}

//-----------------------------------------------------------------------------
// A standard filter to be applied to just about everything.
//-----------------------------------------------------------------------------
bool StandardFilterRules(IHandleEntity* pHandleEntity, int fContentsMask)
{
	C_BaseEntity* pCollide = EntityFromEntityHandle(pHandleEntity);

	// Static prop case...
	if (!pCollide)
		return true;

	SolidType_t solid = static_cast<SolidType_t>(pCollide->m_nSolidType());
	const model_t* pModel = pCollide->GetModel();

	if ((I::ModelInfoClient->GetModelType(pModel) != mod_brush) || (solid != SOLID_BSP && solid != SOLID_VPHYSICS))
	{
		if ((fContentsMask & CONTENTS_MONSTER) == 0)
			return false;
	}

	// This code is used to cull out tests against see-thru entities
	if (!(fContentsMask & CONTENTS_WINDOW) && pCollide->IsTransparent())
		return false;

	// FIXME: this is to skip BSP models that are entities that can be 
	// potentially moved/deleted, similar to a monster but doors don't seem to 
	// be flagged as monsters
	// FIXME: the FL_WORLDBRUSH looked promising, but it needs to be set on 
	// everything that's actually a worldbrush and it currently isn't
	if (!(fContentsMask & CONTENTS_MOVEABLE) && (pCollide->GetMoveType() == MOVETYPE_PUSH))// !(touch->flags & FL_WORLDBRUSH) )
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// Simple trace filter
//-----------------------------------------------------------------------------
CTraceFilterSimple::CTraceFilterSimple(const IHandleEntity* passedict, int collisionGroup,
	ShouldHitFunc_t pExtraShouldHitFunc)
{
	m_pPassEnt = passedict;
	m_collisionGroup = collisionGroup;
	m_pExtraShouldHitCheckFunction = pExtraShouldHitFunc;
}

//-----------------------------------------------------------------------------
// The trace filter!
//-----------------------------------------------------------------------------
bool CTraceFilterSimple::ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
{
	if (!StandardFilterRules(pHandleEntity, contentsMask))
		return false;

	if (m_pPassEnt)
	{
		if (!PassServerEntityFilter(pHandleEntity, m_pPassEnt))
		{
			return false;
		}
	}

	// Don't test if the game code tells us we should ignore this collision...
	C_BaseEntity* pEntity = EntityFromEntityHandle(pHandleEntity);

	if (!pEntity)
		return false;

	if (!pEntity->ShouldCollide(m_collisionGroup, contentsMask))
		return false;

	//if (pEntity && !g_pGameRules->ShouldCollide(m_collisionGroup, pEntity->GetCollisionGroup()))
	//	return false;

	if (m_pExtraShouldHitCheckFunction && (!(m_pExtraShouldHitCheckFunction(pHandleEntity, contentsMask))))
		return false;

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Trace filter that only hits NPCs and the player
//-----------------------------------------------------------------------------
bool CTraceFilterOnlyNPCsAndPlayer::ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
{
	if (CTraceFilterSimple::ShouldHitEntity(pHandleEntity, contentsMask))
	{
		C_BaseEntity* pEntity = EntityFromEntityHandle(pHandleEntity);
		if (!pEntity)
			return false;

#ifdef CSTRIKE_DLL
#ifndef CLIENT_DLL
		if (pEntity->Classify() == CLASS_PLAYER_ALLY)
			return true; // CS hostages are CLASS_PLAYER_ALLY but not IsNPC()
#endif // !CLIENT_DLL
#endif // CSTRIKE_DLL
		return (pEntity->IsNPC() || pEntity->IsPlayer());
	}
	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Trace filter that only hits anything but NPCs and the player
//-----------------------------------------------------------------------------
bool CTraceFilterNoNPCsOrPlayer::ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
{
	if (CTraceFilterSimple::ShouldHitEntity(pHandleEntity, contentsMask))
	{
		C_BaseEntity* pEntity = EntityFromEntityHandle(pHandleEntity);
		if (!pEntity)
			return NULL;
#ifndef CLIENT_DLL
		if (pEntity->Classify() == CLASS_PLAYER_ALLY)
			return false; // CS hostages are CLASS_PLAYER_ALLY but not IsNPC()
#endif
		return (!pEntity->IsNPC() && !pEntity->IsPlayer());
	}
	return false;
}

//-----------------------------------------------------------------------------
// Trace filter that skips two entities
//-----------------------------------------------------------------------------
CTraceFilterSkipTwoEntities::CTraceFilterSkipTwoEntities(const IHandleEntity* passentity, const IHandleEntity* passentity2, int collisionGroup) :
	CTraceFilterSimple(passentity, collisionGroup), m_pPassEnt2(passentity2)
{
}

bool CTraceFilterSkipTwoEntities::ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
{
	assert(pHandleEntity);
	if (!PassServerEntityFilter(pHandleEntity, m_pPassEnt2))
		return false;

	return CTraceFilterSimple::ShouldHitEntity(pHandleEntity, contentsMask);
}


//-----------------------------------------------------------------------------
// Trace filter that can take a list of entities to ignore
//-----------------------------------------------------------------------------
CTraceFilterSimpleList::CTraceFilterSimpleList(int collisionGroup) :
	CTraceFilterSimple(NULL, collisionGroup)
{
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTraceFilterSimpleList::ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
{
	if (m_PassEntities.Find(pHandleEntity) != m_PassEntities.InvalidIndex())
		return false;

	return CTraceFilterSimple::ShouldHitEntity(pHandleEntity, contentsMask);
}


//-----------------------------------------------------------------------------
// Purpose: Add an entity to my list of entities to ignore in the trace
//-----------------------------------------------------------------------------
void CTraceFilterSimpleList::AddEntityToIgnore(IHandleEntity* pEntity)
{
	m_PassEntities.AddToTail(pEntity);
}

//-----------------------------------------------------------------------------
// Trace filter that can take a classname to ignore
//-----------------------------------------------------------------------------
CTraceFilterSkipClassname::CTraceFilterSkipClassname(const IHandleEntity* passentity, const char* pchClassname, int collisionGroup) :
	CTraceFilterSimple(passentity, collisionGroup), m_pchClassname(pchClassname)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTraceFilterSkipClassname::ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
{
	C_BaseEntity* pEntity = EntityFromEntityHandle(pHandleEntity);
	if (!pEntity || FClassnameIs(pEntity, m_pchClassname))
		return false;

	return CTraceFilterSimple::ShouldHitEntity(pHandleEntity, contentsMask);
}

//-----------------------------------------------------------------------------
// Trace filter that skips two classnames
//-----------------------------------------------------------------------------
CTraceFilterSkipTwoClassnames::CTraceFilterSkipTwoClassnames(const IHandleEntity* passentity, const char* pchClassname, const char* pchClassname2, int collisionGroup) :
	CTraceFilterSkipClassname(passentity, pchClassname, collisionGroup), m_pchClassname2(pchClassname2)
{
}

bool CTraceFilterSkipTwoClassnames::ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
{
	C_BaseEntity* pEntity = EntityFromEntityHandle(pHandleEntity);
	if (!pEntity || FClassnameIs(pEntity, m_pchClassname2))
		return false;

	return CTraceFilterSkipClassname::ShouldHitEntity(pHandleEntity, contentsMask);
}

//-----------------------------------------------------------------------------
// Trace filter that can take a list of entities to ignore
//-----------------------------------------------------------------------------
CTraceFilterSimpleClassnameList::CTraceFilterSimpleClassnameList(const IHandleEntity* passentity, int collisionGroup) :
	CTraceFilterSimple(passentity, collisionGroup)
{
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
bool CTraceFilterSimpleClassnameList::ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
{
	C_BaseEntity* pEntity = EntityFromEntityHandle(pHandleEntity);

	if (!pEntity)
		return false;

	for (int i = 0; i < m_PassClassnames.Count(); ++i)
	{
		if (FClassnameIs(pEntity, m_PassClassnames[i]))
			return false;
	}

	return CTraceFilterSimple::ShouldHitEntity(pHandleEntity, contentsMask);
}

//-----------------------------------------------------------------------------
// Purpose: Add an entity to my list of entities to ignore in the trace
//-----------------------------------------------------------------------------
void CTraceFilterSimpleClassnameList::AddClassnameToIgnore(const char* pchClassname)
{
	m_PassClassnames.AddToTail(pchClassname);
}

CTraceFilterChain::CTraceFilterChain(ITraceFilter* pTraceFilter1, ITraceFilter* pTraceFilter2)
{
	m_pTraceFilter1 = pTraceFilter1;
	m_pTraceFilter2 = pTraceFilter2;
}

bool CTraceFilterChain::ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
{
	bool bResult1 = true;
	bool bResult2 = true;

	if (m_pTraceFilter1)
		bResult1 = m_pTraceFilter1->ShouldHitEntity(pHandleEntity, contentsMask);

	if (m_pTraceFilter2)
		bResult2 = m_pTraceFilter2->ShouldHitEntity(pHandleEntity, contentsMask);

	return (bResult1 && bResult2);
}

//-----------------------------------------------------------------------------
// Sweeps against a particular model, using collision rules 
//-----------------------------------------------------------------------------
void UTIL_TraceModel(const Vector& vecStart, const Vector& vecEnd, const Vector& hullMin,
	const Vector& hullMax, C_BaseEntity* pentModel, int collisionGroup, trace_t* ptr)
{
	// Cull it....
	if (pentModel && pentModel->ShouldCollide(collisionGroup, MASK_ALL))
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd, hullMin, hullMax);
		I::EngineTrace->ClipRayToEntity(ray, MASK_ALL, pentModel, ptr);
	}
	else
	{
		memset(ptr, 0, sizeof(trace_t));
		ptr->fraction = 1.0f;
	}
}

bool UTIL_EntityHasMatchingRootParent(C_BaseEntity* pRootParent, C_BaseEntity* pEntity)
{
	if (pRootParent)
	{
		// NOTE: Don't let siblings/parents collide.
		if (pRootParent == pEntity->GetRootMoveParent())
			return true;

		if (pEntity->m_hOwnerEntity() && pRootParent == pEntity->m_hOwnerEntity()->GetRootMoveParent())
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// Sweep an entity from the starting to the ending position 
//-----------------------------------------------------------------------------
class CTraceFilterEntity : public CTraceFilterSimple
{
	//DECLARE_CLASS(CTraceFilterEntity, CTraceFilterSimple);

public:
	CTraceFilterEntity(C_BaseEntity* pEntity, int nCollisionGroup)
		: CTraceFilterSimple(pEntity, nCollisionGroup)
	{
		m_pRootParent = pEntity->GetRootMoveParent();
		m_pEntity = pEntity;
		m_checkHash = false; //g_EntityCollisionHash->IsObjectInHash(pEntity);
	}

	bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
	{
		C_BaseEntity* pEntity = EntityFromEntityHandle(pHandleEntity);

		if (!pEntity)
			return false;

		// Check parents against each other
		// NOTE: Don't let siblings/parents collide.
		if (UTIL_EntityHasMatchingRootParent(m_pRootParent, pEntity))
			return false;

		if (m_checkHash)
		{
			//if (g_EntityCollisionHash->IsObjectPairInHash(m_pEntity, pEntity))
			//	return false;
		}

#ifndef CLIENT_DLL
		if (m_pEntity->IsNPC())
		{
			if (NPC_CheckBrushExclude(m_pEntity, pEntity))
				return false;

		}
#endif

		return CTraceFilterSimple::ShouldHitEntity(pHandleEntity, contentsMask);
	}

private:
	C_BaseEntity* m_pRootParent;
	C_BaseEntity* m_pEntity;
	bool		m_checkHash;
};

class CTraceFilterEntityIgnoreOther : public CTraceFilterEntity
{
	//DECLARE_CLASS(CTraceFilterEntityIgnoreOther, CTraceFilterEntity);
public:
	CTraceFilterEntityIgnoreOther(C_BaseEntity* pEntity, const IHandleEntity* pIgnore, int nCollisionGroup) :
		CTraceFilterEntity(pEntity, nCollisionGroup), m_pIgnoreOther(pIgnore)
	{
	}

	bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
	{
		if (pHandleEntity == m_pIgnoreOther)
			return false;

		return CTraceFilterEntity::ShouldHitEntity(pHandleEntity, contentsMask);
	}

private:
	const IHandleEntity* m_pIgnoreOther;
};

//-----------------------------------------------------------------------------
// Sweeps a particular entity through the world 
//-----------------------------------------------------------------------------
void UTIL_TraceEntity(C_BaseEntity* pEntity, const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, trace_t* ptr)
{
	ICollideable* pCollision = pEntity->m_Collision();

	// Adding this assertion here so game code catches it, but really the assertion belongs in the engine
	// because one day, rotated collideables will work!
	assert(pCollision->GetCollisionAngles() == vec3_angle);

	CTraceFilterEntity traceFilter(pEntity, pCollision->GetCollisionGroup());

#ifdef PORTAL
	UTIL_Portal_TraceEntity(pEntity, vecAbsStart, vecAbsEnd, mask, &traceFilter, ptr);
#else
	I::EngineTrace->SweepCollideable(pCollision, vecAbsStart, vecAbsEnd, pCollision->GetCollisionAngles(), mask, &traceFilter, ptr);
#endif
}

void UTIL_TraceEntity(C_BaseEntity* pEntity, const Vector& vecAbsStart, const Vector& vecAbsEnd,
	unsigned int mask, const IHandleEntity* pIgnore, int nCollisionGroup, trace_t* ptr)
{
	ICollideable* pCollision;
	pCollision = pEntity->GetCollideable();

	// Adding this assertion here so game code catches it, but really the assertion belongs in the engine
	// because one day, rotated collideables will work!
	assert(pCollision->GetCollisionAngles() == vec3_angle);

	CTraceFilterEntityIgnoreOther traceFilter(pEntity, pIgnore, nCollisionGroup);

#ifdef PORTAL
	UTIL_Portal_TraceEntity(pEntity, vecAbsStart, vecAbsEnd, mask, &traceFilter, ptr);
#else
	I::EngineTrace->SweepCollideable(pCollision, vecAbsStart, vecAbsEnd, pCollision->GetCollisionAngles(), mask, &traceFilter, ptr);
#endif
}

void UTIL_TraceEntity(C_BaseEntity* pEntity, const Vector& vecAbsStart, const Vector& vecAbsEnd,
	unsigned int mask, ITraceFilter* pFilter, trace_t* ptr)
{
	ICollideable* pCollision;
	pCollision = pEntity->GetCollideable();

	// Adding this assertion here so game code catches it, but really the assertion belongs in the engine
	// because one day, rotated collideables will work!
	assert(pCollision->GetCollisionAngles() == vec3_angle);

#ifdef PORTAL
	UTIL_Portal_TraceEntity(pEntity, vecAbsStart, vecAbsEnd, mask, pFilter, ptr);
#else
	I::EngineTrace->SweepCollideable(pCollision, vecAbsStart, vecAbsEnd, pCollision->GetCollisionAngles(), mask, pFilter, ptr);
#endif
}

// ----
// This is basically a regular TraceLine that uses the FilterEntity filter.
void UTIL_TraceLineFilterEntity(C_BaseEntity* pEntity, const Vector& vecAbsStart, const Vector& vecAbsEnd,
	unsigned int mask, int nCollisionGroup, trace_t* ptr)
{
	CTraceFilterEntity traceFilter(pEntity, nCollisionGroup);
	UTIL_TraceLine(vecAbsStart, vecAbsEnd, mask, &traceFilter, ptr);
}

void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	trace_t playerTrace;
	Ray_t ray;
	float smallestFraction = tr->fraction;
	const float maxRange = 60.0f;

	ray.Init(vecAbsStart, vecAbsEnd);

	for (int k = 1; k <= I::GlobalVars->maxClients; ++k)
	{
		C_BasePlayer* player = UTIL_PlayerByIndex(k);

		if (!player || !player->IsAlive())
			continue;

#ifdef CLIENT_DLL
		if (player->IsDormant())
			continue;
#endif // CLIENT_DLL

		if (filter && filter->ShouldHitEntity(player, mask) == false)
			continue;

		float range = DistanceToRay(player->WorldSpaceCenter(), vecAbsStart, vecAbsEnd);
		if (range < 0.0f || range > maxRange)
			continue;

		I::EngineTrace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, player, &playerTrace);
		if (playerTrace.fraction < smallestFraction)
		{
			// we shortened the ray - save off the trace
			*tr = playerTrace;
			smallestFraction = playerTrace.fraction;
		}
	}
}

bool UTIL_IsLowViolence(void)
{
	return I::EngineClient->IsLowViolence();
}

bool UTIL_ShouldShowBlood(int color)
{
	if (color != DONT_BLEED)
	{
		if (color == BLOOD_COLOR_RED)
		{
			return true;
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool UTIL_IsSpaceEmpty(C_BaseEntity* pMainEnt, const Vector& vMin, const Vector& vMax)
{
	Vector vHalfDims = (vMax - vMin) * 0.5f;
	Vector vCenter = vMin + vHalfDims;

	trace_t trace;
	UTIL_TraceHull(vCenter, vCenter, -vHalfDims, vHalfDims, MASK_SOLID, pMainEnt, COLLISION_GROUP_NONE, &trace);

	bool bClear = (trace.fraction == 1 && trace.allsolid != 1 && (trace.startsolid != 1));
	return bClear;
}

void UTIL_StringToFloatArray(float* pVector, int count, const char* pString)
{
	char* pstr, * pfront, tempString[128];
	int	j;

	Q_strncpy(tempString, pString, sizeof(tempString));
	pstr = pfront = tempString;

	for (j = 0; j < count; j++)			// lifted from pr_edict.c
	{
		pVector[j] = atof(pfront);

		// skip any leading whitespace
		while (*pstr && *pstr <= ' ')
			pstr++;

		// skip to next whitespace
		while (*pstr && *pstr > ' ')
			pstr++;

		if (!*pstr)
			break;

		pstr++;
		pfront = pstr;
	}
	for (j++; j < count; j++)
	{
		pVector[j] = 0;
	}
}

void UTIL_StringToVector(float* pVector, const char* pString)
{
	UTIL_StringToFloatArray(pVector, 3, pString);
}

void UTIL_StringToIntArray(int* pVector, int count, const char* pString)
{
	char* pstr, * pfront, tempString[128];
	int	j;

	Q_strncpy(tempString, pString, sizeof(tempString));
	pstr = pfront = tempString;

	for (j = 0; j < count; j++)			// lifted from pr_edict.c
	{
		pVector[j] = atoi(pfront);

		while (*pstr && *pstr != ' ')
			pstr++;
		if (!*pstr)
			break;
		pstr++;
		pfront = pstr;
	}

	for (j++; j < count; j++)
	{
		pVector[j] = 0;
	}
}

void UTIL_StringToColor32(color32* color, const char* pString)
{
	int tmp[4];
	UTIL_StringToIntArray(tmp, 4, pString);
	color->r = tmp[0];
	color->g = tmp[1];
	color->b = tmp[2];
	color->a = tmp[3];
}

#ifndef _XBOX
void UTIL_DecodeICE(unsigned char* buffer, int size, const unsigned char* key)
{
	if (!key)
		return;

	IceKey ice(0); // level 0 = 64bit key
	ice.set(key); // set key

	int blockSize = ice.blockSize();

	unsigned char* temp = (unsigned char*)_alloca(PAD_NUMBER(size, blockSize));
	unsigned char* p1 = buffer;
	unsigned char* p2 = temp;

	// encrypt data in 8 byte blocks
	int bytesLeft = size;
	while (bytesLeft >= blockSize)
	{
		ice.decrypt(p1, p2);
		bytesLeft -= blockSize;
		p1 += blockSize;
		p2 += blockSize;
	}

	// copy encrypted data back to original buffer
	Q_memcpy(buffer, temp, size - bytesLeft);
}
#endif

// work-around since client header doesn't like inlined gpGlobals->curtime
float IntervalTimer::Now(void) const
{
	return I::GlobalVars->curtime;
}

// work-around since client header doesn't like inlined gpGlobals->curtime
float CountdownTimer::Now(void) const
{
	return I::GlobalVars->curtime;
}

IClientEntity* UTIL_EntityByIndex(int entindex)
{
	return I::ClientEntityList->GetClientEntity(entindex);
}

#ifdef CLIENT_DLL

C_BasePlayer* UTIL_PlayerByIndex(int entindex)
{
	IClientEntity* pEntity = UTIL_EntityByIndex(entindex);

	return pEntity ? pEntity->As<C_BasePlayer*>() : nullptr;
}

C_TFPlayer* UTIL_TFPlayerByIndex(int entindex)
{
	IClientEntity* pEntity = UTIL_EntityByIndex(entindex);

	return pEntity ? pEntity->As<C_TFPlayer*>() : nullptr;
}

#endif

C_BasePlayer* UTIL_PlayerByName(const char* name)
{
	if (!name || !name[0])
		return NULL;

	for (int i = 1; i <= I::GlobalVars->maxClients; i++)
	{
		C_BasePlayer* pPlayer = UTIL_PlayerByIndex(i);

		if (!pPlayer)
			continue;

#ifndef CLIENT_DLL
		if (!pPlayer->IsConnected())
			continue;
#endif

		if (Q_stricmp(pPlayer->GetPlayerName(), name) == 0)
		{
			return pPlayer;
		}
	}

	return NULL;
}

// Finds a player who has this non-ambiguous substring
C_BasePlayer* UTIL_PlayerByPartialName(const char* name)
{
	if (!name || !name[0])
		return NULL;

	C_BasePlayer* pFound = NULL;
	for (int i = 1; i <= I::GlobalVars->maxClients; i++)
	{
		C_BasePlayer* pPlayer = UTIL_PlayerByIndex(i);

		if (!pPlayer)
			continue;

#ifndef CLIENT_DLL
		if (!pPlayer->IsConnected())
			continue;
#endif

		if (Q_stristr(pPlayer->GetPlayerName(), name))
		{
			if (pFound)
			{
				// Ambiguous
				return NULL;
			}

			pFound = pPlayer;
		}
	}

	return pFound;
}

C_BasePlayer* UTIL_PlayerByUserId(int userID)
{
	for (int i = 1; i <= I::GlobalVars->maxClients; i++)
	{
		C_BasePlayer* pPlayer = UTIL_PlayerByIndex(i);

		if (!pPlayer)
			continue;

#ifndef CLIENT_DLL
		if (!pPlayer->IsConnected())
			continue;
#endif

		if (pPlayer->GetUserID() == userID)
		{
			return pPlayer;
		}
	}

	return NULL;
}

int UTIL_StringFieldToInt(const char* szValue, const char** pValueStrings, int iNumStrings)
{
	if (!szValue || !szValue[0])
		return -1;

	for (int i = 0; i < iNumStrings; i++)
	{
		if (V_strcmp(szValue, pValueStrings[i]))
			return i;
	}

	assert(0);
	return -1;
}

int find_day_of_week(struct tm& found_day, int day_of_week, int step)
{
	return 0;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
#ifdef USES_ECON_ITEMS
static bool					  s_HolidaysCalculated = false;
static CBitVec<kHolidayCount> s_HolidaysActive;

//-----------------------------------------------------------------------------
// Purpose: Used at level change and round start to re-calculate which holiday is active
//-----------------------------------------------------------------------------
void UTIL_CalculateHolidays()
{
	s_HolidaysActive.ClearAll();

	CRTime::UpdateRealTime();
	for (int iHoliday = 0; iHoliday < kHolidayCount; iHoliday++)
	{
		if (EconHolidays_IsHolidayActive(iHoliday, CRTime::RTime32TimeCur()))
		{
			s_HolidaysActive.Set(iHoliday);
		}
	}

	s_HolidaysCalculated = true;
}
#endif // USES_ECON_ITEMS

bool UTIL_IsHolidayActive( /*EHoliday*/ int eHoliday)
{
#ifdef USES_ECON_ITEMS
	if (IsX360())
		return false;

	if (!s_HolidaysCalculated)
	{
		UTIL_CalculateHolidays();
	}

	return s_HolidaysActive.IsBitSet(eHoliday);
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int	UTIL_GetHolidayForString(const char* pszHolidayName)
{
#ifdef USES_ECON_ITEMS
	if (!pszHolidayName)
		return kHoliday_None;

	return EconHolidays_GetHolidayForString(pszHolidayName);
#else
	return 0;
#endif
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
const char* UTIL_GetActiveHolidayString()
{
#ifdef USES_ECON_ITEMS
	return EconHolidays_GetActiveHolidayString();
#else
	return NULL;
#endif
}

/// Clamp and round float vals to int.  The values are in the 0...255 range.
Color FloatRGBAToColor(float r, float g, float b, float a)
{
	return Color(
		(unsigned char)Clamp(r + 0.5f, 0.0f, 255.0f),
		(unsigned char)Clamp(g + 0.5f, 0.0f, 255.0f),
		(unsigned char)Clamp(b + 0.5f, 0.0f, 255.0f),
		(unsigned char)Clamp(a + 0.5f, 0.0f, 255.0f)
	);
}

float LerpFloat(float x0, float x1, float t)
{
	return x0 + (x1 - x0) * t;
}

Color LerpColor(const Color& c0, const Color& c1, float t)
{
	if (t <= 0.0f) return c0;
	if (t >= 1.0f) return c1;
	return FloatRGBAToColor(
		LerpFloat((float)c0.r(), (float)c1.r(), t),
		LerpFloat((float)c0.g(), (float)c1.g(), t),
		LerpFloat((float)c0.b(), (float)c1.b(), t),
		LerpFloat((float)c0.a(), (float)c1.a(), t)
	);
}

bool FClassnameIs(IClientEntity* pEntity, const char* const szClassName)
{
	if (!pEntity)
		return false;

	const auto pCC = pEntity->GetClientClass();

	if (!pCC)
		return false;

	return (V_strcmp(pCC->GetName(), szClassName) == 0);
}

//ISteamUtils* GetSteamUtils()
//{
//#ifdef GAME_DLL
//	// Use steamgameserver context if this isn't a client/listenserver.
//	if (engine->IsDedicatedServer())
//	{
//		return steamgameserverapicontext ? steamgameserverapicontext->SteamGameServerUtils() : NULL;
//	}
//#endif
//	return steamapicontext ? steamapicontext->SteamUtils() : NULL;
//}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
//EUniverse GetUniverse()
//{
//	if (!GetSteamUtils())
//		return k_EUniverseInvalid;
//
//	static EUniverse steamUniverse = GetSteamUtils()->GetConnectedUniverse();
//	return steamUniverse;
//}