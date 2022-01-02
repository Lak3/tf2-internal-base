#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace ViewRender
	{
		namespace PerformScreenSpaceEffects
		{
			inline Hook::CFunction Func;
			using FN = void(__fastcall*)(void*, void*, int, int, int, int);

			void __fastcall Detour(void* ecx, void* edx, int x, int y, int w, int h);
		}

		void Initialize();
	}
}