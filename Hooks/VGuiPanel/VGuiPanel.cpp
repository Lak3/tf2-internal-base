#include "VGuiPanel.h"

using namespace Hooks;

void __fastcall VGuiPanel::PaintTraverse::Detour(void* ecx, void* edx, vgui::VPANEL vguiPanel, bool forceRepaint, bool allowForce)
{
	Table.Original<FN>(Index)(ecx, edx, vguiPanel, forceRepaint, allowForce);
}

void VGuiPanel::Initialize()
{
	XASSERT(Table.Initialize(I::VGuiPanel) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&PaintTraverse::Detour, PaintTraverse::Index) == FAILED_TO_HOOK);
}