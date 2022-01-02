#include "CL_Main.h"

#include "../../Features/Vars.h"

using namespace Hooks;

CPureServerWhitelist* __cdecl CL_Main::CL_LoadWhitelist::Detour(INetworkStringTable* pTable, const char* pName)
{
	return Vars::Misc::BypassPure ? NULL : Func.Original<FN>()(pTable, pName);
}

void __cdecl CL_Main::CL_Move::Detour(float accumulated_extra_samples, bool bFinalTick)
{
	Func.Original<FN>()(accumulated_extra_samples, bFinalTick);
}

void __fastcall CL_Main::CL_SendMove::Detour(void* ecx, void* edx)
{
	Func.Original<FN>()(ecx, edx);
}

void CL_Main::Initialize()
{
	//CL_LoadWhitelist
	{
		using namespace CL_LoadWhitelist;

		const FN pfCLLoadWhitelist = reinterpret_cast<FN>(g_Offsets.m_dwCLLoadWhitelist);
		XASSERT(pfCLLoadWhitelist == nullptr);

		if (pfCLLoadWhitelist)
			XASSERT(Func.Initialize(pfCLLoadWhitelist, &Detour) == FAILED_TO_INITIALIZE);
	}

	//CL_Move
	{
		using namespace CL_Move;

		const FN pfCLMove = reinterpret_cast<FN>(g_Offsets.m_dwCLMove);
		XASSERT(pfCLMove == nullptr);

		if (pfCLMove)
			XASSERT(Func.Initialize(pfCLMove, &Detour) == FAILED_TO_INITIALIZE);
	}

	//CL_SendMove
	{
		//using namespace CL_SendMove;

		//const FN pfCLSendMove = reinterpret_cast<FN>(g_Offsets.m_dwCLSendMove);
		//XASSERT(pfCLSendMove == nullptr);

		//if (pfCLSendMove)
		//	XASSERT(Func.Initialize(pfCLSendMove, &Detour) == FAILED_TO_INITIALIZE);
	}
}