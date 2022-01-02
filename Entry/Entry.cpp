#include "Entry.h"

#define _VALIDATE(x) if (!x) { Error(#x); m_bShouldUnload = true; return; }

void CModuleEntry::Load()
{
	while (!GetModuleHandleW(L"mss32.dll"))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	MathLib_Init();

	_VALIDATE(g_Offsets.Initialize());
	_VALIDATE(g_Interfaces.Initialize());

	_VALIDATE(G::ConVars.Initialize());
	_VALIDATE(G::Draw.Initialize());

	G::Draw.ReloadFonts();
	G::Draw.ReloadScreenSize();

	if (I::EngineClient->IsInGame())
		g_Globals.m_nLocalIndex = I::EngineClient->GetLocalPlayer();

	_VALIDATE(Hooks::Initialize());

	//I::EngineClient->ClientCmd_Unrestricted("r_drawtracers_firstperson 1");
}

void CModuleEntry::Unload()
{
	_VALIDATE(Hooks::UnInitialize());

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	G::Draw.UnInitialize();

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

bool CModuleEntry::ShouldUnload()
{
	if (GetAsyncKeyState(VK_F11))
		m_bShouldUnload = true;

	return m_bShouldUnload;
}