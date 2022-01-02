#include "BaseEntity.h"

#include "../../Features/Vars.h"

using namespace Hooks;

int __fastcall BaseEntity::BaseInterpolatePart1::Detour(C_BaseEntity* pThis, void* edx, float& currentTime, Vector& oldOrigin, QAngle& oldAngles, Vector& oldVel, int& bNoMoreChanges)
{
	return Func.Original<FN>()(pThis, edx, currentTime, oldOrigin, oldAngles, oldVel, bNoMoreChanges);
}

void BaseEntity::Initialize()
{
	//BaseInterpolatePart1
	{
		using namespace BaseInterpolatePart1;

		const FN pfBaseInterpPart1 = reinterpret_cast<FN>(g_Offsets.m_dwBaseInterpolatePart1);
		XASSERT(pfBaseInterpPart1 == nullptr);

		if (pfBaseInterpPart1)
			XASSERT(Func.Initialize(pfBaseInterpPart1, &Detour) == FAILED_TO_INITIALIZE);
	}
}