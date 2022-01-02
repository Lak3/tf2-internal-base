#pragma once

#include "../../SDK/SDK.h"

/*

	virtual void			LevelShutdown(void) = 0;

*/

namespace Hooks
{
	namespace BaseClient
	{
		inline Hook::CTable Table;

		namespace LevelInitPreEntity
		{
			using FN = void(__fastcall*)(void*, void*, char const*);
			constexpr uint32 Index = 5u;

			void __fastcall Detour(void* ecx, void* edx, char const* pMapName);
		}

		namespace LevelInitPostEntity
		{
			using FN = void(__fastcall*)(void*, void*);
			constexpr uint32 Index = 6u;

			void __fastcall Detour(void* ecx, void* edx);
		}

		namespace LevelShutdown
		{
			using FN = void(__fastcall*)(void*, void*);
			constexpr uint32 Index = 7u;

			void __fastcall Detour(void* ecx, void* edx);
		}

		namespace CreateMove
		{
			using FN = void(__fastcall*)(void*, void*, int, float, bool);
			constexpr uint32 Index = 21u;

			void __fastcall Detour(void* ecx, void* edx, int sequence_number, float input_sample_frametime, bool active);
		}

		namespace FrameStageNotify
		{
			using FN = void(__fastcall*)(void*, void*, ClientFrameStage_t);
			constexpr uint32 Index = 35u;

			void __fastcall Detour(void* ecx, void* edx, ClientFrameStage_t curStage);
		}

		namespace DispatchUserMessage
		{
			using FN = bool(__fastcall*)(void*, void*, int, bf_read&);
			constexpr uint32 Index = 36u;

			bool __fastcall Detour(void* ecx, void* edx, int msg_type, bf_read& msg_data);
		}

		void Initialize();
	}
}