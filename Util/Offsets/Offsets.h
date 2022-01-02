#pragma once

#include "../Pattern/Pattern.h"

class COffsets
{
public:
	bool Initialize();

public:
	DWORD m_dwGlobalVars = 0x0;
	DWORD m_dwPredictionRandomSeed = 0x0;
	DWORD m_dwStartDrawing = 0x0;
	DWORD m_dwFinishDrawing = 0x0;
	DWORD m_dwEntitySphereQuery = 0x0;
	DWORD m_dwClientModeShared = 0x0;
	DWORD m_dwIsPlayerOnSteamFriendsList = 0x0;
	DWORD m_dwPhysicsRunThink = 0x0;
	DWORD m_dwSaveData = 0x0;
	DWORD m_dwOnAddEntity = 0x0;
	DWORD m_dwOnRemoveEntity = 0x0;
	DWORD m_dwCheckForSequenceChange = 0x0;
	DWORD m_dwLoadFromBuffer = 0x0;
	DWORD m_dwInitializeKeyValue = 0x0;
	DWORD m_dwPerformScreenSpaceEffects = 0x0;
	DWORD m_dwCLMove = 0x0;
	DWORD m_dwCLSendMove = 0x0;
	DWORD m_dwCLLoadWhitelist = 0x0;
	DWORD m_dwClientState = 0x0;
	DWORD m_dwGetTracerType = 0x0;
	DWORD m_dwFireBullet = 0x0;
	DWORD m_dwBaseInterpolatePart1 = 0x0;
	DWORD m_dwSendDataGram = 0x0;
	DWORD m_dwShutdown = 0x0;
	DWORD m_dwSendNetMsg = 0x0;
	DWORD m_dwCalcPlayerView = 0x0;
	DWORD m_dwBaseAnimatingSetupBones = 0x0;
};

inline COffsets g_Offsets;