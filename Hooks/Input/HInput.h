#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace Input
	{
		inline Hook::CTable Table;

		namespace WriteUsercmdDeltaToBuffer
		{
			using FN = bool(__fastcall*)(void*, void*, bf_write*, int, int, bool);
			constexpr uint32 Index = 5u;

			bool __fastcall Detour(void* ecx, void* edx, bf_write* buf, int from, int to, bool isnewcommand);
		}

		namespace EncodeUserCmdToBuffer
		{
			using FN = void(__fastcall*)(void*, void*, bf_write&, int);
			constexpr uint32 Index = 6u;

			void __fastcall Detour(void* ecx, void* edx, bf_write& buf, int slot);
		}

		namespace DecodeUserCmdFromBuffer
		{
			using FN = void(__fastcall*)(void*, void*, bf_read& buf, int slot);
			constexpr uint32 Index = 7u;

			void __fastcall Detour(void* ecx, void* edx, bf_read& buf, int slot);
		}

		namespace GetUserCmd
		{
			using FN = CUserCmd* (__fastcall*)(void*, void*, int);
			constexpr uint32 Index = 8u;

			CUserCmd* __fastcall Detour(void* ecx, void* edx, int sequence_number);
		}

		void Initialize();
	}
}