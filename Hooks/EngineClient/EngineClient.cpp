#include "EngineClient.h"

using namespace Hooks;

bool __fastcall EngineClient::IsPlayingTimeDemo::Detour(void* ecx, void* edx)
{
	return Table.Original<FN>(Index)(ecx, edx);
}

void EngineClient::Initialize()
{
	XASSERT(Table.Initialize(I::EngineClient) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&IsPlayingTimeDemo::Detour, IsPlayingTimeDemo::Index) == FAILED_TO_HOOK);
}