#include "ViewRender.h"

using namespace Hooks;

void __fastcall ViewRender::PerformScreenSpaceEffects::Detour(void* ecx, void* edx, int x, int y, int w, int h)
{
	Func.Original<FN>()(ecx, edx, x, y, w, h);
}

void ViewRender::Initialize()
{
	//PerformScreenSpaceEffects
	{
		using namespace PerformScreenSpaceEffects;

		const FN pfPerformScreenSpaceFx = reinterpret_cast<FN>(*reinterpret_cast<DWORD*>(g_Offsets.m_dwPerformScreenSpaceEffects) + g_Offsets.m_dwPerformScreenSpaceEffects + 0x04);
		XASSERT(pfPerformScreenSpaceFx == nullptr);

		if (pfPerformScreenSpaceFx)
			XASSERT(Func.Initialize(pfPerformScreenSpaceFx, &Detour) == FAILED_TO_INITIALIZE);
	}
}