#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace ClientMode
	{
		inline Hook::CTable Table;

		namespace OverrideView
		{
			using FN = void(__fastcall*)(void*, void*, CViewSetup* pSetup);
			constexpr uint32 Index = 16u;

			void __fastcall Detour(void* ecx, void* edx, CViewSetup* pSetup);
		}

		namespace CreateMove
		{
			using FN = bool(__fastcall*)(void*, void*, float, CUserCmd*);
			constexpr uint32 Index = 21u;

			bool __fastcall Detour(void* ecx, void* edx, float flInputSampleTime, CUserCmd* cmd);
		}

		namespace ShouldDrawViewModel
		{
			using FN = bool(__fastcall*)(void*, void*);
			constexpr uint32 Index = 24u;

			bool __fastcall Detour(void* ecx, void* edx);
		}

		namespace GetViewModelFOV
		{
			using FN = float(__fastcall*)(void*, void*);
			constexpr uint32 Index = 32u;

			float __fastcall Detour(void* ecx, void* edx);
		}

		namespace DoPostScreenSpaceEffects
		{
			using FN = bool(__fastcall*)(void*, void*, const CViewSetup*);
			constexpr uint32 Index = 39u;

			bool __fastcall Detour(void* ecx, void* edx, const CViewSetup* pSetup);
		}
		
		namespace ShouldBlackoutAroundHUD
		{
			using FN = bool(__fastcall*)(void*, void*);
			constexpr uint32 Index = 42u;

			bool __fastcall Detour(void* ecx, void* edx);
		}

		void Initialize();
	}
}