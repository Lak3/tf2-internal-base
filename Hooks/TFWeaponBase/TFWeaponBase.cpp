#include "TFWeaponBase.h"

#include "../../Features/Vars.h"

using namespace Hooks;

const char* __fastcall TFWeaponBase::GetTracerType::Detour(C_TFWeaponBase* pThis, void* edx)
{
	return Vars::Visual::Tracers ? "merasmus_zap" : Func.Original<FN>()(pThis, edx);
}

void TFWeaponBase::Initialize()
{
	//GetTracerType
	{
		using namespace GetTracerType;

		const FN pfGetTracerType = reinterpret_cast<FN>(g_Offsets.m_dwGetTracerType);
		XASSERT(pfGetTracerType == nullptr);

		if (pfGetTracerType)
			XASSERT(Func.Initialize(pfGetTracerType, &Detour) == FAILED_TO_INITIALIZE);
	}
}