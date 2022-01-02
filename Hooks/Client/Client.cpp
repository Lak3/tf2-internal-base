#include "Client.h"

#include "../../Features/Vars.h"
#include "../NetChannel/NetChannel.h"

using namespace Hooks;

void __fastcall BaseClient::LevelInitPreEntity::Detour(void* ecx, void* edx, char const* pMapName)
{
	Table.Original<FN>(Index)(ecx, edx, pMapName);
}

void __fastcall BaseClient::LevelInitPostEntity::Detour(void* ecx, void* edx)
{
	Table.Original<FN>(Index)(ecx, edx);

	g_Globals.m_nLocalIndex = I::EngineClient->GetLocalPlayer();
}

void __fastcall BaseClient::LevelShutdown::Detour(void* ecx, void* edx)
{
	g_Globals.m_bIsInGame = false;
	g_Globals.m_bIsGameUIVisible = true;

	g_Globals.m_nLocalIndex = -1;
	g_Globals.m_nMaxClients = -1;
	g_Globals.m_nMaxEntities = -1;

	Table.Original<FN>(Index)(ecx, edx);
}

void __fastcall BaseClient::CreateMove::Detour(void* ecx, void* edx, int sequence_number, float input_sample_frametime, bool active)
{
	Table.Original<FN>(Index)(ecx, edx, sequence_number, input_sample_frametime, active);
}

void __fastcall BaseClient::FrameStageNotify::Detour(void* ecx, void* edx, ClientFrameStage_t curStage)
{
	Table.Original<FN>(Index)(ecx, edx, curStage);

	if (curStage == ClientFrameStage_t::FRAME_NET_UPDATE_END)
	{
		g_Globals.m_bIsInGame = I::EngineClient->IsInGame();
		g_Globals.m_bIsGameUIVisible = I::EngineVGui->IsGameUIVisible();

		if (g_Globals.m_bIsInGame)
		{
			g_Globals.m_nMaxClients = I::EngineClient->GetMaxClients();
			g_Globals.m_nMaxEntities = I::ClientEntityList->GetMaxEntities();
		}
	}
}

bool __fastcall BaseClient::DispatchUserMessage::Detour(void* ecx, void* edx, int msg_type, bf_read& msg_data)
{
	return Table.Original<FN>(Index)(ecx, edx, msg_type, msg_data);
}

void BaseClient::Initialize()
{
	XASSERT(Table.Initialize(I::BaseClient) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&LevelInitPreEntity::Detour, LevelInitPreEntity::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&LevelInitPostEntity::Detour, LevelInitPostEntity::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&LevelShutdown::Detour, LevelShutdown::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&CreateMove::Detour, CreateMove::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&FrameStageNotify::Detour, FrameStageNotify::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&DispatchUserMessage::Detour, DispatchUserMessage::Index) == FAILED_TO_HOOK);
}