#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace BasePlayer
	{
		namespace CalcPlayerView
		{
			inline Hook::CFunction Func;
			using FN = void(__fastcall*)(C_BasePlayer*, void*, Vector&, QAngle&, float&);

			void __fastcall Detour(C_BasePlayer* pThis, void* edx, Vector& eyeOrigin, QAngle& eyeAngles, float& fov);
		}

		void Initialize();
	}
}