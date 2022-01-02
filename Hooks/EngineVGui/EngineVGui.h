#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace EngineVGui
	{
		inline Hook::CTable Table;

		namespace Paint
		{
			using FN = void(__fastcall*)(void*, void*, int);
			constexpr uint32 Index = 13u;

			void __fastcall Detour(void* ecx, void* edx, int mode);
		}

		void Initialize();
	}
}