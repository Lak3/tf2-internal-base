#include "VGuiSurface.h"

void __fastcall Hooks::VGuiSurface::OnScreenSizeChanged::Detour(void* ecx, void* edx, int oldWidth, int oldHeight)
{
	Table.Original<FN>(Index)(ecx, edx, oldWidth, oldHeight);

	G::Draw.ReloadFonts();
	G::Draw.ReloadScreenSize();
}

void __fastcall Hooks::VGuiSurface::LockCursor::Detour(void* ecx, void* edx)
{
	Table.Original<FN>(Index)(ecx, edx);
}

void Hooks::VGuiSurface::Initialize()
{
	XASSERT(Table.Initialize(I::VGuiSurface) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&OnScreenSizeChanged::Detour, OnScreenSizeChanged::Index) == FAILED_TO_HOOK);
	XASSERT(Table.Hook(&LockCursor::Detour, LockCursor::Index) == FAILED_TO_HOOK);
}
