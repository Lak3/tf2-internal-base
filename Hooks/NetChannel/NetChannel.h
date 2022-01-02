#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace NetChannel
	{
		namespace SendNetMsg
		{
			inline Hook::CFunction Func;
			using FN = bool(__fastcall*)(CNetChan*, void*, INetMessage&, bool, bool);

			bool __fastcall Detour(CNetChan* pThis, void* edx, INetMessage& msg, bool bForceReliable, bool bVoice);
		}

		namespace SendDataGram
		{
			inline Hook::CFunction Func;
			using FN = int(__fastcall*)(INetChannel*, void*, bf_write*);

			int __fastcall Detour(INetChannel* pThis, void* edx, bf_write* data);
		}

		namespace Shutdown
		{
			inline Hook::CFunction Func;
			using FN = void(__fastcall*)(INetChannel*, void*, const char*);

			void __fastcall Detour(INetChannel* pThis, void* edx, const char* pReason);
		}

		void Initialize();
	}
}