#include "EngineVGui.h"

#include "../../Features/ESP/ESP.h"

using namespace Hooks;

#define DBG(...) G::Draw.String(EFonts::DEBUG, 5, nY, COLOR_GREY, TXT_DEFAULT, __VA_ARGS__); nY += G::Draw.GetFontHeight(EFonts::DEBUG)

void __fastcall EngineVGui::Paint::Detour(void* ecx, void* edx, int mode)
{
	static const auto pfStartDrawing = reinterpret_cast<void(__thiscall*)(IMatSystemSurface*)>(g_Offsets.m_dwStartDrawing);
	static const auto pfFinishDrawing = reinterpret_cast<void(__thiscall*)(IMatSystemSurface*)>(g_Offsets.m_dwFinishDrawing);

	Table.Original<FN>(Index)(ecx, edx, mode);

	if (mode & PaintMode_t::PAINT_UIPANELS)
	{
		G::Draw.ReloadMatrix();

		pfStartDrawing(I::MatSystemSurface);
		{
			if (g_Globals.m_bIsInGame && !g_Globals.m_bIsGameUIVisible)
			{
				C_TFPlayer* pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex);

				if (pLocal)
				{
					F::ESP.Render(pLocal);
				}
			}

			G::Draw.String(EFonts::DEBUG, 5, 5, { 204, 204, 204, 255 }, TXT_DEFAULT, L"Polonium TF2");
		}
		pfFinishDrawing(I::MatSystemSurface);
	}
}

void EngineVGui::Initialize()
{
	XASSERT(Table.Initialize(I::EngineVGui) == FAILED_TO_INITIALIZE);
	XASSERT(Table.Hook(&Paint::Detour, Paint::Index) == FAILED_TO_HOOK);
}