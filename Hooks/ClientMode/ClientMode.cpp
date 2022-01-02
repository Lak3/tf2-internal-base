#include "ClientMode.h"

#include "../../Features/Vars.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Prediction/Prediction.h"

#include "../CL_Main/CL_Main.h"

using namespace Hooks;

void __fastcall ClientMode::OverrideView::Detour(void* ecx, void* edx, CViewSetup* pSetup)
{
	Table.Original<FN>(Index)(ecx, edx, pSetup);
}

bool __fastcall ClientMode::CreateMove::Detour(void* ecx, void* edx, float flInputSampleTime, CUserCmd* cmd)
{
	if (!cmd || cmd->command_number <= 0)
		return Table.Original<FN>(Index)(ecx, edx, flInputSampleTime, cmd);

	uintptr_t _bp; __asm mov _bp, ebp;
	bool* pbSendPacket = (bool*)(***(uintptr_t***)_bp - 0x1);

	C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

	if (pLocal && pLocal->IsAlive())
	{
		C_TFWeaponBase* pWeapon = pLocal->GetActiveTFWeapon();

		if (pWeapon)
		{
			F::Prediction.Start(pLocal, cmd);
			{
				//Run aimbot, triggerbot etc. here
			}
			F::Prediction.Finish(pLocal);
		}

		F::Misc.Run(pLocal, cmd);
	}

	return false;
}

bool __fastcall ClientMode::ShouldDrawViewModel::Detour(void* ecx, void* edx)
{
	return Table.Original<FN>(Index)(ecx, edx);
}

float __fastcall ClientMode::GetViewModelFOV::Detour(void* ecx, void* edx)
{
	return Table.Original<FN>(Index)(ecx, edx);
}

bool __fastcall ClientMode::DoPostScreenSpaceEffects::Detour(void* ecx, void* edx, const CViewSetup* pSetup)
{
	return Table.Original<FN>(Index)(ecx, edx, pSetup);
}

bool __fastcall ClientMode::ShouldBlackoutAroundHUD::Detour(void* ecx, void* edx)
{
	return Table.Original<FN>(Index)(ecx, edx);
}

void ClientMode::Initialize()
{
	XASSERT(Table.Initialize(I::ClientMode) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&OverrideView::Detour, OverrideView::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&CreateMove::Detour, CreateMove::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&ShouldDrawViewModel::Detour, ShouldDrawViewModel::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&GetViewModelFOV::Detour, GetViewModelFOV::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&DoPostScreenSpaceEffects::Detour, DoPostScreenSpaceEffects::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&ShouldBlackoutAroundHUD::Detour, ShouldBlackoutAroundHUD::Index) == FAILED_TO_HOOK);
}