#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace EngineClient
	{
		inline Hook::CTable Table;

		namespace IsPlayingTimeDemo
		{
			using FN = bool(__fastcall*)(void*, void*);
			constexpr uint32 Index = 78u;

			bool __fastcall Detour(void* ecx, void* edx);
		}

		void Initialize();
	}
}