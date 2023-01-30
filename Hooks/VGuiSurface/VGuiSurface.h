#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace VGuiSurface
	{
		inline Hook::CTable Table;

		namespace OnScreenSizeChanged
		{
			using FN = void(__fastcall*)(void*, void*, int, int);
			constexpr uint32 Index = 111u;

			void __fastcall Detour(void* ecx, void* edx, int oldWidth, int oldHeight);
		}

		namespace LockCursor
		{
			using FN = void(__fastcall*)(void*, void*);
			constexpr uint32 Index = 62u;

			void __fastcall Detour(void* ecx, void* edx);
		}

		void Initialize();
	}
}