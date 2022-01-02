#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace TFPlayer
	{
		namespace FireBullet
		{
			inline Hook::CFunction Func;
			using FN = void(__fastcall*)(C_TFPlayer*, void*, C_TFWeaponBase*, const FireBulletsInfo_t&, bool, int, int);

			void __fastcall Detour(C_TFPlayer* pThis, void* edx, C_TFWeaponBase* pWpn, FireBulletsInfo_t& info, bool bDoEffects, int nDamageType, int nCustomDamageType);
		}

		void Initialize();
	}
}