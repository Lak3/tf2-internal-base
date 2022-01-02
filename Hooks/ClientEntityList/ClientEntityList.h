#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace ClientEntityList
	{
		namespace OnAddEntity
		{
			inline Hook::CFunction Func;

			using FN = void(__fastcall*)(void*, void*, IHandleEntity*, CBaseHandle);

			void __fastcall Detour(void* ecx, void* edx, IHandleEntity* pEnt, CBaseHandle handle);
		}

		namespace OnRemoveEntity
		{
			inline Hook::CFunction Func;

			using FN = void(__fastcall*)(void*, void*, IHandleEntity*, CBaseHandle);

			void __fastcall Detour(void* ecx, void* edx, IHandleEntity* pEnt, CBaseHandle handle);
		}

		void Initialize();
	}
}