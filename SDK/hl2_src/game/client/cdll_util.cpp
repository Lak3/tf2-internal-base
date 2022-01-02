//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $Workfile:     $
// $NoKeywords: $
//=============================================================================//
#include "hud.h"
#include "cdll_util.h"
#include "../../public/materialsystem/imaterial.h"
#include "../../public/materialsystem/itexture.h"
#include "../../public/materialsystem/imaterialsystem.h"
#include "../shared/imovehelper.h"
#include "../../public/tier1/checksum_crc.h"
#include "../../public/r_efx.h"
#include "../../public/model_types.h"
#include "../../public/engine/IEngineTrace.h"
#include "../../public/engine/ivmodelinfo.h"
#include "../../public/vgui/ISurface.h"
#include "../../public/inputsystem/iinputsystem.h"
#include "../../public/globalvars_base.h"
#include "../../public/cdll_int.h"
#include "../../public/icliententitylist.h"
#include "../../public/vphysics_interface.h"
#include "../../public/worldsize.h"
#include "tf/c_tf_player.h"

//-----------------------------------------------------------------------------
// ConVars
//-----------------------------------------------------------------------------
bool g_MakingDevShots = false;

//-----------------------------------------------------------------------------
// Purpose: Performs a var args printf into a static return buffer
// Input  : *format - 
//			... - 
// Output : char
//-----------------------------------------------------------------------------
char* VarArgs(const char* format, ...)
{
	va_list		argptr;
	static char	string[1024];

	va_start(argptr, format);
	Q_vsnprintf(string, sizeof(string), format, argptr);
	va_end(argptr);

	return string;
}

//-----------------------------------------------------------------------------
// Purpose: Returns true if the entity index corresponds to a player slot 
// Input  : index - 
// Output : bool
//-----------------------------------------------------------------------------
bool IsPlayerIndex(int index)
{
	return (index >= 1 && index <= I::GlobalVars->maxClients) ? true : false;
}

int GetLocalPlayerIndex(void)
{
	C_BasePlayer* player = C_BasePlayer::GetLocalPlayer();

	if (player)
		return player->entindex();
	else
		return 0; //game not started yet
}

bool IsLocalPlayerSpectator(void)
{
	C_BasePlayer* player = C_BasePlayer::GetLocalPlayer();

	if (player)
		return player->GetObserverTarget() ? true : false;
	else
		return false;	// game not started yet
}

int GetSpectatorMode(void)
{
	C_BasePlayer* player = C_BasePlayer::GetLocalPlayer();

	if (player)
		return player->GetObserverMode();
	else
		return OBS_MODE_NONE;	// game not started yet
}

int GetSpectatorTarget(void)
{
	C_BasePlayer* player = C_BasePlayer::GetLocalPlayer();

	if (player)
	{
		C_BaseEntity* target = player->GetObserverTarget();

		if (target)
			return target->entindex();
		else
			return 0;
	}
	else
	{
		return 0; // game not started yet
	}
}

int GetLocalPlayerTeam(void)
{
	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();

	if (pPlayer)
		return pPlayer->GetTeamNumber();
	else
		return TEAM_UNASSIGNED;
}

//-----------------------------------------------------------------------------
// Purpose: Convert angles to -180 t 180 range
// Input  : angles - 
//-----------------------------------------------------------------------------
void NormalizeAngles(QAngle& angles)
{
	int i;

	// Normalize angles to -180 to 180 range
	for (i = 0; i < 3; i++)
	{
		if (angles[i] > 180.0f)
		{
			angles[i] -= 360.0f;
		}
		else if (angles[i] < -180.0f)
		{
			angles[i] += 360.0f;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Convert angle to -180 t 180 range
// Input  : angle - 
//-----------------------------------------------------------------------------
void NormalizeAngle(float& flAngle)
{
	if (flAngle > 180.0f)
	{
		flAngle -= 360.0f;
	}
	else if (flAngle < -180.0f)
	{
		flAngle += 360.0f;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Convert angles to be in legitimate bounds
// Input  : angles - 
//-----------------------------------------------------------------------------
void ClampAngles(QAngle& va)
{
	va.x = Clamp(va.x, -89.0f, 89.0f);
	NormalizeAngle(va.y);
	va.z = 0.0f;
}

QAngle GetAngleToPosition(const Vector& source, const Vector& destination)
{
	const Vector delta = (source - destination);
	QAngle angles = { (::atanf(delta.z / FastSqrt((delta.x * delta.x) + (delta.y * delta.y))) * 57.295779513082f), (::atanf(delta.y / delta.x) * 57.295779513082f), 0.0f };

	if (delta.x >= 0.0f)
		angles.y += 180.0f;

	return angles;
}

float GetFovToPosition(const QAngle& vFrom, const QAngle& vTo)
{
	Vector vSrc;
	AngleVectors(vFrom, &vSrc);

	Vector vDst;
	AngleVectors(vTo, &vDst);

	float flRet = RAD2DEG(::acosf(DotProduct(vDst, vSrc) / vDst.LengthSqr()));

	if (!::isfinite(flRet) || ::isinf(flRet) || ::isnan(flRet))
		flRet = FLT_MAX;

	return flRet;
}

//-----------------------------------------------------------------------------
// Purpose: Interpolate Euler angles using quaternions to avoid singularities
// Input  : start - 
//			end - 
//			output - 
//			frac - 
//-----------------------------------------------------------------------------
void InterpolateAngles(const QAngle& start, const QAngle& end, QAngle& output, float frac)
{
	Quaternion src, dest;

	// Convert to quaternions
	AngleQuaternion(start, src);
	AngleQuaternion(end, dest);

	Quaternion result;

	// Slerp
	QuaternionSlerp(src, dest, frac, result);

	// Convert to euler
	QuaternionAngles(result, output);
}

//-----------------------------------------------------------------------------
// Purpose: Simple linear interpolation
// Input  : frac - 
//			src - 
//			dest - 
//			output - 
//-----------------------------------------------------------------------------
void InterpolateVector(float frac, const Vector& src, const Vector& dest, Vector& output)
{
	int i;

	for (i = 0; i < 3; i++)
	{
		output[i] = src[i] + frac * (dest[i] - src[i]);
	}
}

client_textmessage_t* TextMessageGet(const char* pName)
{
	return I::EngineClient->TextMessageGet(pName);
}

//-----------------------------------------------------------------------------
// Purpose: ScreenHeight returns the height of the screen, in pixels
// Output : int
//-----------------------------------------------------------------------------
int ScreenHeight(void)
{
	return g_Globals.m_nScreenHeight;
}

//-----------------------------------------------------------------------------
// Purpose: ScreenWidth returns the width of the screen, in pixels
// Output : int
//-----------------------------------------------------------------------------
int ScreenWidth(void)
{
	return g_Globals.m_nScreenWidht;
}

//-----------------------------------------------------------------------------
// Purpose: Return the difference between two angles
// Input  : destAngle - 
//			srcAngle - 
// Output : float
//-----------------------------------------------------------------------------
float UTIL_AngleDiff(float destAngle, float srcAngle)
{
	float delta;

	delta = destAngle - srcAngle;

	if (destAngle > srcAngle)
	{
		while (delta >= 180)
			delta -= 360;
	}
	else
	{
		while (delta <= -180)
			delta += 360;
	}

	return delta;
}

float UTIL_WaterLevel(const Vector& position, float minz, float maxz)
{
	Vector midUp = position;
	midUp.z = minz;

	if (!(UTIL_PointContents(midUp) & MASK_WATER))
		return minz;

	midUp.z = maxz;
	if (UTIL_PointContents(midUp) & MASK_WATER)
		return maxz;

	float diff = maxz - minz;
	while (diff > 1.0)
	{
		midUp.z = minz + diff / 2.0;
		if (UTIL_PointContents(midUp) & MASK_WATER)
		{
			minz = midUp.z;
		}
		else
		{
			maxz = midUp.z;
		}
		diff = maxz - minz;
	}

	return midUp.z;
}

char TEXTURETYPE_Find(trace_t* ptr)
{
	surfacedata_t* psurfaceData = I::PhysicsSurfaceProps->GetSurfaceData(static_cast<int>(ptr->surface.surfaceProps));

	return psurfaceData->game.material;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
int UTIL_PrecacheDecal(const char* name, bool preload)
{
	return I::EngineEffects->Draw_DecalIndexFromName((char*)name);
}

static csurface_t	g_NullSurface = { "**empty**", 0 };
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void UTIL_SetTrace(trace_t& trace, const Ray_t& ray, C_BaseEntity* ent, float fraction, int hitgroup, unsigned int contents, const Vector& normal, float intercept)
{
	trace.startsolid = (fraction == 0.0f);
	trace.fraction = fraction;
	VectorCopy(ray.m_Start, trace.startpos);
	VectorMA(ray.m_Start, fraction, ray.m_Delta, trace.endpos);
	VectorCopy(normal, trace.plane.normal);
	trace.plane.dist = intercept;
	trace.m_pEnt = ent;
	trace.hitgroup = hitgroup;
	trace.surface = g_NullSurface;
	trace.contents = contents;
}

//-----------------------------------------------------------------------------
// Purpose: Get the x & y positions of a world position in screenspace
//			Returns true if it's onscreen
//-----------------------------------------------------------------------------
bool GetVectorInScreenSpace(Vector pos, int& iX, int& iY, Vector* vecOffset)
{
	Vector screen;

	// Apply the offset, if one was specified
	if (vecOffset != NULL)
		pos += *vecOffset;

	// Transform to screen space
	const int iFacing = ScreenTransform(pos, screen);
	iX = 0.5f * (1.0f + screen[0]) * ScreenWidth();
	iY = 0.5f * (1.0f - screen[1]) * ScreenHeight();

	// Make sure the player's facing it
	if (iFacing)
	{
		// We're actually facing away from the Target. Stomp the screen position.
		iX = -640;
		iY = -640;
		return false;
	}

	return true;
}

bool GetVectorInScreenSpace(Vector pos, Vector2D& vScreen, Vector* vecOffset)
{
	int x, y;

	const bool bRet = GetVectorInScreenSpace(pos, x, y, vecOffset);

	vScreen = { static_cast<vec_t>(x), static_cast<vec_t>(y) };

	return bRet;
}

//-----------------------------------------------------------------------------
// Purpose: Get the x & y positions of a world position in HUD space
//			Returns true if it's onscreen
//-----------------------------------------------------------------------------
bool GetVectorInHudSpace(Vector pos, int& iX, int& iY, Vector* vecOffset)
{
	Vector screen;

	// Apply the offset, if one was specified
	if (vecOffset != NULL)
		pos += *vecOffset;

	// Transform to HUD space
	const int iFacing = HudTransform(pos, screen);
	iX = 0.5f * (1.0f + screen[0]) * ScreenWidth();
	iY = 0.5f * (1.0f - screen[1]) * ScreenHeight();

	// Make sure the player's facing it
	if (iFacing)
	{
		// We're actually facing away from the Target. Stomp the screen position.
		iX = -640;
		iY = -640;
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Get the x & y positions of an entity in screenspace
//			Returns true if it's onscreen
//-----------------------------------------------------------------------------
bool GetTargetInScreenSpace(C_BaseEntity* pTargetEntity, int& iX, int& iY, Vector* vecOffset)
{
	return GetVectorInScreenSpace(pTargetEntity->WorldSpaceCenter(), iX, iY, vecOffset);
}

//-----------------------------------------------------------------------------
// Purpose: Get the x & y positions of an entity in Vgui space
//			Returns true if it's onscreen
//-----------------------------------------------------------------------------
bool GetTargetInHudSpace(C_BaseEntity* pTargetEntity, int& iX, int& iY, Vector* vecOffset)
{
	return GetVectorInHudSpace(pTargetEntity->WorldSpaceCenter(), iX, iY, vecOffset);
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *player - 
//			msg_dest - 
//			*msg_name - 
//			*param1 - 
//			*param2 - 
//			*param3 - 
//			*param4 - 
//-----------------------------------------------------------------------------
void ClientPrint(C_BasePlayer* player, int msg_dest, const char* msg_name, const char* param1 /*= NULL*/, const char* param2 /*= NULL*/, const char* param3 /*= NULL*/, const char* param4 /*= NULL*/)
{

}

CEntitySphereQuery::CEntitySphereQuery(const Vector& center, float radius, int flagMask, int partitionMask)
{
	reinterpret_cast<void(__thiscall*)(void*, const Vector&, float, int, int)>(g_Offsets.m_dwEntitySphereQuery)(this, center, radius, flagMask, partitionMask);
}

C_BaseEntity* CEntitySphereQuery::GetCurrentEntity()
{
	if (m_listIndex < m_listCount)
		return m_pList[m_listIndex];

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : font - 
//			*str - 
// Output : int
//-----------------------------------------------------------------------------
int UTIL_ComputeStringWidth(vgui::HFont& font, const char* str)
{
	float pixels = 0;
	const char* p = str;
	const char* pAfter = p + 1;
	const char* pBefore = "\0";

	while (*p)
	{
#if USE_GETKERNEDCHARWIDTH
		float wide, abcA;
		vgui::surface()->GetKernedCharWidth(font, *p, *pBefore, *pAfter, wide, abcA);
		pixels += wide;
#else
		pixels += I::VGuiSurface->GetCharacterWidth(font, *p);
#endif
		pBefore = p;
		p++;
		if (*p)
			pAfter = p + 1;
		else
			pAfter = "\0";
	}

	return static_cast<int>(ceilf(pixels));
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : font - 
//			*str - 
// Output : int
//-----------------------------------------------------------------------------
int UTIL_ComputeStringWidth(vgui::HFont& font, const wchar_t* str)
{
	float pixels = 0;
	const wchar_t* p = str;
	const wchar_t* pAfter = p + 1;
	const wchar_t* pBefore = L"\0";

	while (*p)
	{
#if USE_GETKERNEDCHARWIDTH
		float wide, abcA;
		vgui::surface()->GetKernedCharWidth(font, *p, *pBefore, *pAfter, wide, abcA);
		pixels += wide;
#else
		pixels += I::VGuiSurface->GetCharacterWidth(font, *p);
#endif
		pBefore = p;
		p++;
		if (*p)
			pAfter = p + 1;
		else
			pAfter = L"\0";
	}

	return static_cast<int>(ceilf(pixels));
}

//-----------------------------------------------------------------------------
//Purpose: Scans player names
//Passes the player name to be checked in a KeyValues pointer
//with the keyname "name"
// - replaces '&' with '&&' so they will draw in the scoreboard
// - replaces '#' at the start of the name with '*'
//-----------------------------------------------------------------------------
void UTIL_MakeSafeName(const char* oldName, char* newName, int newNameBufSize)
{
	assert(newNameBufSize >= sizeof(newName[0]));

	int newpos = 0;

	for (const char* p = oldName; *p != 0 && newpos < newNameBufSize - 1; p++)
	{
		//check for a '#' char at the beginning
		if (p == oldName && *p == '#')
		{
			newName[newpos] = '*';
			newpos++;
		}
		else if (*p == '%')
		{
			// remove % chars
			newName[newpos] = '*';
			newpos++;
		}
		else if (*p == '&')
		{
			//insert another & after this one
			if (newpos + 2 < newNameBufSize)
			{
				newName[newpos] = '&';
				newName[newpos + 1] = '&';
				newpos += 2;
			}
		}
		else
		{
			newName[newpos] = *p;
			newpos++;
		}
	}
	newName[newpos] = 0;
}

//-----------------------------------------------------------------------------
// Purpose: Scans player names and replaces characters that vgui won't
//          display properly
// Input  : *oldName - player name to be fixed up
// Output : *char - static buffer with the safe name
//-----------------------------------------------------------------------------
const char* UTIL_SafeName(const char* oldName)
{
	static char safeName[MAX_PLAYER_NAME_LENGTH * 2 + 1];
	UTIL_MakeSafeName(oldName, safeName, sizeof(safeName));

	return safeName;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *buffer - 
//-----------------------------------------------------------------------------
void UTIL_FreeFile(byte* buffer)
{
	delete[] buffer;
}

//-----------------------------------------------------------------------------
// Purpose: Given a vector, clamps the scalar axes to MAX_COORD_FLOAT ranges from worldsize.h
// Input  : *pVecPos - 
//-----------------------------------------------------------------------------
void UTIL_BoundToWorldSize(Vector* pVecPos)
{
	assert(pVecPos);
	for (int i = 0; i < 3; ++i)
	{
		(*pVecPos)[i] = Clamp((*pVecPos)[i], MIN_COORD_FLOAT, MAX_COORD_FLOAT);
	}
}

#ifdef _X360
#define MAP_KEY_FILE_DIR	"cfg"
#else
#define MAP_KEY_FILE_DIR	"media"
#endif

//-----------------------------------------------------------------------------
// Purpose: Returns the filename to count map loads in
//-----------------------------------------------------------------------------
bool UTIL_GetMapLoadCountFileName(const char* pszFilePrependName, char* pszBuffer, int iBuflen)
{
	if (IsX360())
	{
#ifdef _X360
		if (XBX_GetStorageDeviceId() == XBX_INVALID_STORAGE_ID || XBX_GetStorageDeviceId() == XBX_STORAGE_DECLINED)
			return false;
#endif
	}

	if (IsX360())
	{
		Q_snprintf(pszBuffer, iBuflen, "%s:/%s", MAP_KEY_FILE_DIR, pszFilePrependName);
	}
	else
	{
		Q_snprintf(pszBuffer, iBuflen, "%s/%s", MAP_KEY_FILE_DIR, pszFilePrependName);
	}

	return true;
}

#ifdef TF_CLIENT_DLL
#define MAP_KEY_FILE "viewed.res"
#else
#define MAP_KEY_FILE "mapkeys.res"
#endif	