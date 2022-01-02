#include "BasePlayer.h"

#include "../../Features/Vars.h"

using namespace Hooks;

void __fastcall BasePlayer::CalcPlayerView::Detour(C_BasePlayer* pThis, void* edx, Vector& eyeOrigin, QAngle& eyeAngles, float& fov)
{
	if (Vars::Visual::RemoveVisualRecoil && pThis) //Did this in l4d2 when Spookman told me to, so doing it here comes from him.
	{
		const QAngle vOldPunch = pThis->m_vecPunchAngle();

		pThis->m_vecPunchAngle().Init();
		Func.Original<FN>()(pThis, edx, eyeOrigin, eyeAngles, fov);
		pThis->m_vecPunchAngle() = vOldPunch;
	}
	else
	{
		Func.Original<FN>()(pThis, edx, eyeOrigin, eyeAngles, fov);
	}
}

void BasePlayer::Initialize()
{
	//CalcPlayerView
	{
		using namespace CalcPlayerView;

		const FN pfCalcPlayerView = reinterpret_cast<FN>(g_Offsets.m_dwCalcPlayerView);
		XASSERT(pfCalcPlayerView == nullptr);

		if (pfCalcPlayerView)
			XASSERT(Func.Initialize(pfCalcPlayerView, &Detour) == FAILED_TO_INITIALIZE);
	}
}