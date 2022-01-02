#include "TFPlayer.h"

#include "../../Features/Vars.h"

using namespace Hooks;

void __fastcall TFPlayer::FireBullet::Detour(C_TFPlayer* pThis, void* edx, C_TFWeaponBase* pWpn, FireBulletsInfo_t& info, bool bDoEffects, int nDamageType, int nCustomDamageType)
{
	if (Vars::Visual::Tracers)
	{
		C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

		//Thanks to Spook for telling me this.
		if (pThis == pLocal)
		{
			//Remove crit type so tracers some tracers don't break.
			if (nDamageType & DMG_CRITICAL)
				nDamageType &= (~DMG_CRITICAL);

			//Draws tracer on each shot
			info.m_iTracerFreq = 1;
		}
	}

	Func.Original<FN>()(pThis, edx, pWpn, info, bDoEffects, nDamageType, nCustomDamageType);
}

void TFPlayer::Initialize()
{
	//FireBullet
	{
		using namespace FireBullet;

		const FN pfFireBullet = reinterpret_cast<FN>(g_Offsets.m_dwFireBullet);
		XASSERT(pfFireBullet == nullptr);

		if (pfFireBullet)
			XASSERT(Func.Initialize(pfFireBullet, &Detour) == FAILED_TO_INITIALIZE);
	}
}