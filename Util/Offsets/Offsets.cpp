#include "Offsets.h"

#include "../../SDK/hl2_src/public/tier0/platform.h"

#define _CHECKPATT(x) if (x) { Error(#x); return false; }
#define CHECKPATT(x) _CHECKPATT(x == 0x0)

bool COffsets::Initialize()
{
	m_dwEntitySphereQuery = g_Pattern.Find("client.dll", "55 8B EC 83 EC 14 D9 45 0C");
	CHECKPATT(m_dwEntitySphereQuery);

	m_dwIsPlayerOnSteamFriendsList = g_Pattern.Find("client.dll", "55 8B EC 81 EC ? ? ? ? E8 ? ? ? ? 85 C0 74 7E 8B 4D 08 85 C9 74 77 A1 ? ? ? ? 83 78 08 00 74 6C 83 78 0C 00 74 66 A1 ? ? ? ? 83 C1 08 56 8B 30 8D 85 ? ? ? ? 50 8B 01");
	CHECKPATT(m_dwIsPlayerOnSteamFriendsList);

	m_dwPhysicsRunThink = g_Pattern.Find("client.dll", "55 8B EC 53 8B D9 56 57 8B 83 ? ? ? ? C1 E8 16 A8 01 B0 01 0F 85 ? ? ? ?");
	CHECKPATT(m_dwPhysicsRunThink);

	m_dwSaveData = g_Pattern.Find("client.dll", "55 8B EC 83 EC 7C 53 57 8B 7D 0C 8B D9 83 FF FF 75 0B");
	CHECKPATT(m_dwSaveData);

	m_dwOnAddEntity = g_Pattern.Find("client.dll", "55 8B EC 51 8B 45 0C 53 25 ? ? ? ? 56 8B F1 57 8B 7D 08 8D 1C C5 ? ? ? ? 03 DE 89 5D FC 3D ? ? ? ? 73 1D");
	CHECKPATT(m_dwOnAddEntity);

	m_dwOnRemoveEntity = g_Pattern.Find("client.dll", "55 8B EC 51 8B 45 0C 25 ? ? ? ? 53 8B D9 56 8D 0C C5 ? ? ? ? 03 CB 89 4D FC 57 3D ? ? ? ? 73 1B");
	CHECKPATT(m_dwOnRemoveEntity);

	m_dwCheckForSequenceChange = g_Pattern.Find("client.dll", "55 8B EC 53 8B 5D 08 57 8B F9 85 DB 0F 84 ? ? ? ? 83 7F 0C 00 75 05 E8 ? ? ? ? 6B 4F 0C 2C 0F 57 C0 56 8B 37 83 C6 D4 03 F1 F3 0F 10 4E ? 0F 2E C8 9F F6 C4 44 7B 62 8B 45 0C 39 06 75 06 80 7D 10 00 74 55 50 8B CB E8 ? ? ? ? 89 45 10 F6 40 0C 02 75 2B 80 7D 14 00 74 25 FF 36 8B CB E8 ? ? ? ? F3 0F 10 40 ? 8B 45 10 F3 0F 10 48 ? 0F 2F C8 77 03 0F 28 C1 F3 0F 11 46 ? EB 07 C7 47 ? ? ? ? ? 8B CF E8 ? ? ? ? 6B 4F 0C 2C 8B 37 83 C6 D4 03 F1 C7 06 ? ? ? ? C7 46 ? ? ? ? ? C7 46 ? ? ? ? ?");
	CHECKPATT(m_dwCheckForSequenceChange);

	m_dwGetTracerType = g_Pattern.Find("client.dll", "55 8B EC 51 53 56 57 8B F9 E8 ? ? ? ? 6A 00 68 ? ? ? ? 68 ? ? ? ? 6A 00 50 E8 ? ? ? ? 8B 0D ? ? ? ? 83 C4 14 8D B0 ? ? ? ? 83 79 30 00 0F 84 ? ? ? ?");
	CHECKPATT(m_dwGetTracerType);

	m_dwFireBullet = g_Pattern.Find("client.dll", "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 56 57 8B F9 8B 4B 0C F3 0F 10 41 ? F3 0F 10 49 ? 8B 41 04");
	CHECKPATT(m_dwFireBullet);

	m_dwBaseInterpolatePart1 = g_Pattern.Find("client.dll", "55 8B EC 53 8B 5D 18 56 8B F1 C7 03 ? ? ? ? F6 46 7C 01 74 3C");
	CHECKPATT(m_dwBaseInterpolatePart1);

	m_dwCalcPlayerView = g_Pattern.Find("client.dll", "55 8B EC 83 EC 18 53 56 8B F1 8B 0D ? ? ? ? 57 8B 01 8B 40 38 FF D0 84 C0 75 0B");
	CHECKPATT(m_dwCalcPlayerView);

	m_dwBaseAnimatingSetupBones = g_Pattern.Find("client.dll", "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 33 C9 33 D2 89 4D EC 89 55 F0 8B 46 08 85 C0 74 3B");
	CHECKPATT(m_dwBaseAnimatingSetupBones);

	m_dwLoadFromBuffer = g_Pattern.Find("engine.dll", "55 8B EC 83 EC 38 53 8B 5D 0C");
	CHECKPATT(m_dwLoadFromBuffer);

	m_dwCLMove = g_Pattern.Find("engine.dll", "55 8B EC 83 EC ? 83 3D ? ? ? ? 02 0F 8C ? ? 00 00 E8 ? ? ? 00 84 C0");
	CHECKPATT(m_dwCLMove);

	m_dwCLSendMove = g_Pattern.Find("engine.dll", "55 8B EC 81 EC ? ? ? ? A1 ? ? ? ? 8D 4D CC 56 57 8B 3D ? ? ? ? 40 03 F8 C6 45 B0 01 C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? C7 45 ? ? ? ? ? E8 ? ? ? ? 8D 4D E4 E8 ? ? ? ? 6A FF 6A 00 68 ? ? ? ? 8D 85 ? ? ? ? C6 45 B0 00 50 8D 4D E4 E8 ? ? ? ? A1 ? ? ? ? 83 C0 01 C7 45 ? ? ? ? ? 79 04");
	CHECKPATT(m_dwCLSendMove);

	m_dwCLLoadWhitelist = g_Pattern.Find("engine.dll", "55 8B EC 83 EC 30 56 8B 75 08 8B CE FF 75 0C 8B 06 FF 50 30 8B C8 81 F9 ? ? ? ? 74 56");
	CHECKPATT(m_dwCLLoadWhitelist);

	m_dwSendDataGram = g_Pattern.Find("engine.dll", "55 8B EC B8 ? ? ? ? E8 ? ? ? ? A1 ? ? ? ? 53 56 8B D9 83 78 30 00 57 8B 7D 08 89 5D D0 74 2E");
	CHECKPATT(m_dwSendDataGram);

	m_dwShutdown = g_Pattern.Find("engine.dll", "55 8B EC 83 EC 10 56 8B F1 83 BE ? ? ? ? ? 0F 8C ? ? ? ?");
	CHECKPATT(m_dwShutdown);

	m_dwSendNetMsg = g_Pattern.Find("engine.dll", "56 8B F1 8B 46 0C C7 06 ? ? ? ? 85 C0 74 12");
	CHECKPATT(m_dwSendNetMsg);

	m_dwStartDrawing = g_Pattern.Find("vguimatsurface.dll", "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14");
	CHECKPATT(m_dwStartDrawing);
	
	m_dwFinishDrawing = g_Pattern.Find("vguimatsurface.dll", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00");
	CHECKPATT(m_dwFinishDrawing);

	if (const DWORD dwClientMode = g_Pattern.Find("client.dll", "8B 0D ? ? ? ? 8B 02 D9 05"))
		m_dwClientModeShared = (dwClientMode + 0x02);

	if (const DWORD dwScreenSpaceFx = g_Pattern.Find("client.dll", "E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 83 F8 01 75 64 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B F0 85 FF 74 0E 8B 17"))
		m_dwPerformScreenSpaceEffects = (dwScreenSpaceFx + 0x01);

	if (const DWORD dwGlobalVars = g_Pattern.Find("engine.dll", "A1 ? ? ? ? 8B 11 68"))
		m_dwGlobalVars = (dwGlobalVars + 0x08);

	if (const DWORD dwClientState = g_Pattern.Find("engine.dll", "68 ? ? ? ? E8 ? ? ? ? 83 C4 08 5F 5E 5B 5D C3"))
		m_dwClientState = (dwClientState + 0x01);

	if (const DWORD dwInitKVal = g_Pattern.Find("engine.dll", "FF 15 ? ? ? ? 83 C4 08 89 06 8B C6"))
		m_dwInitializeKeyValue = (dwInitKVal - 0x42);

	if (const DWORD dwRandomSeed = g_Pattern.Find("client.dll", "C7 05 ? ? ? ? ? ? ? ? 5D C3 8B 40 34"))
		m_dwPredictionRandomSeed = (dwRandomSeed + 0x02);

	CHECKPATT(m_dwGlobalVars);
	CHECKPATT(m_dwPredictionRandomSeed);
	CHECKPATT(m_dwInitializeKeyValue);
	CHECKPATT(m_dwClientModeShared);
	CHECKPATT(m_dwClientState);

	//If we arrive here, return success.
	return true;
}