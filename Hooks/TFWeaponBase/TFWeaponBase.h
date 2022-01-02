#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace TFWeaponBase
	{
		namespace GetTracerType
		{
			inline Hook::CFunction Func;
			using FN = const char* (__fastcall*)(C_TFWeaponBase*, void*);

			const char* __fastcall Detour(C_TFWeaponBase* pThis, void* edx);
		}

		void Initialize();
	}
}