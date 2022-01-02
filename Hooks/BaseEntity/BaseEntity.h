#pragma once

#include "../../SDK/SDK.h"

namespace Hooks
{
	namespace BaseEntity
	{
		namespace BaseInterpolatePart1
		{
			inline Hook::CFunction Func;
			using FN = int(__fastcall*)(C_BaseEntity*, void*, float&, Vector&, QAngle&, Vector&, int&);

			int __fastcall Detour(C_BaseEntity* pThis, void* edx, float& currentTime, Vector& oldOrigin, QAngle& oldAngles, Vector& oldVel, int& bNoMoreChanges);
		}

		void Initialize();
	}
}