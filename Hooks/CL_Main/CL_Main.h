#pragma once

#include "../../SDK/SDK.h"

class INetworkStringTable;
class CPureServerWhitelist;

inline Vector g_vShiftStartOrigin = { };
inline Vector g_vShiftStartVelocity = { };

namespace Hooks
{
	namespace CL_Main
	{
		namespace CL_LoadWhitelist
		{
			inline Hook::CFunction Func;
			using FN = CPureServerWhitelist* (__cdecl*)(INetworkStringTable*, const char*);

			CPureServerWhitelist* __cdecl Detour(INetworkStringTable* pTable, const char* pName);
		}

		namespace CL_Move
		{
			inline Hook::CFunction Func;
			using FN = void(__cdecl*)(float, bool);
			
			void __cdecl Detour(float accumulated_extra_samples, bool bFinalTick);
		}

		namespace CL_SendMove
		{
			inline Hook::CFunction Func;
			using FN = void(__fastcall*)(void*, void*);

			void __fastcall Detour(void* ecx, void* edx);
		}

		void Initialize();
	}
}