#include "DrawManager.h"

bool CDraw::Initialize()
{
	//Idk if using UtlMap makes any sense at all.

	if (EFonts::FONT_LAST > static_cast<EFonts>(9))
	{
		Error(L"You have modified the EFonts enum, remember to do the same in CDraw::Initialize() - (DrawManager.cpp)!!!");
		return false;
	}

	m_Fonts.SetLessFunc(DefLessFunc(EFonts));

	m_Fonts.Insert(EFonts::DEBUG);
	m_Fonts.Insert(EFonts::ESP);
	m_Fonts.Insert(EFonts::ESP_NAME);
	m_Fonts.Insert(EFonts::ESP_WEAPON);
	m_Fonts.Insert(EFonts::MENU_TAHOMA);
	m_Fonts.Insert(EFonts::MENU_CONSOLAS);
	m_Fonts.Insert(EFonts::MENU_VERDANA);
	m_Fonts.Insert(EFonts::MENU_ARIAL);
	m_Fonts.Insert(EFonts::MENU_TAB);

	//If we arrive here, return success
	return true;
}

void CDraw::UnInitialize()
{
	m_Fonts.Purge();
}

void CDraw::ReloadFonts()
{
	m_Fonts.Element(m_Fonts.Find(EFonts::DEBUG))         = { "Consolas",  16, FW_DONTCARE, vgui::ISurface::EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts.Element(m_Fonts.Find(EFonts::ESP))           = { "Tahoma",    11, FW_DONTCARE, vgui::ISurface::EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts.Element(m_Fonts.Find(EFonts::ESP_NAME))      = { "Arial",     14, FW_DONTCARE, vgui::ISurface::EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts.Element(m_Fonts.Find(EFonts::ESP_WEAPON))    = { "Verdana",   12, FW_DONTCARE, vgui::ISurface::EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts.Element(m_Fonts.Find(EFonts::MENU_TAHOMA))   = { "Tahoma",    12, FW_DONTCARE, vgui::ISurface::EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts.Element(m_Fonts.Find(EFonts::MENU_CONSOLAS)) = { "Consolas",  12, FW_DONTCARE, vgui::ISurface::EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts.Element(m_Fonts.Find(EFonts::MENU_VERDANA))  = { "Verdana",   12, FW_DONTCARE, vgui::ISurface::EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts.Element(m_Fonts.Find(EFonts::MENU_ARIAL))    = { "Arial",     12, FW_DONTCARE, vgui::ISurface::EFontFlags::FONTFLAG_OUTLINE };
	m_Fonts.Element(m_Fonts.Find(EFonts::MENU_TAB))      = { "Verdana",   30, FW_HEAVY   , vgui::ISurface::EFontFlags::FONTFLAG_OUTLINE | vgui::ISurface::EFontFlags::FONTFLAG_ANTIALIAS };

	FOR_EACH_MAP(m_Fonts, n)
	{
		auto& v = m_Fonts[n];

		I::MatSystemSurface->SetFontGlyphSet(v.m_hFont = I::MatSystemSurface->CreateFont(), v.m_szName, v.m_nTall, v.m_nWeight, 0, 0, v.m_nFlags);
	}
}

void CDraw::String(const EFonts& font, int x, int y, const Color& clr, const short align, const char* const str, ...)
{
	va_list va_alist;
	char cbuffer[1024] = { '\0' };
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vsprintf_s(cbuffer, str, va_alist);
	va_end(va_alist);

	wsprintfW(wstr, L"%hs", cbuffer);

	const vgui::HFont fnt = m_Fonts.Element(m_Fonts.Find(font)).m_hFont;

	if (align)
	{
		int w = 0, h = 0;
		I::MatSystemSurface->GetTextSize(fnt, wstr, w, h);

		if (align & TXT_LEFT)
			x -= w;

		if (align & TXT_TOP)
			y -= h;

		if (align & TXT_CENTERX)
			x -= (w / 2);

		if (align & TXT_CENTERY)
			y -= (h / 2);
	}

	I::MatSystemSurface->DrawSetTextPos(x, y);
	I::MatSystemSurface->DrawSetTextFont(fnt);
	I::MatSystemSurface->DrawSetTextColor(clr);
	I::MatSystemSurface->DrawPrintText(wstr, V_wcslen(wstr));
}

void CDraw::String(const EFonts& font, int x, int y, const Color& clr, const short align, const wchar_t* const str, ...)
{
	va_list va_alist;
	wchar_t wstr[1024] = { '\0' };

	va_start(va_alist, str);
	vswprintf_s(wstr, str, va_alist);
	va_end(va_alist);

	const vgui::HFont fnt = m_Fonts.Element(m_Fonts.Find(font)).m_hFont;

	if (align)
	{
		int w = 0, h = 0;
		I::MatSystemSurface->GetTextSize(fnt, wstr, w, h);

		if (align & TXT_LEFT)
			x -= w;

		if (align & TXT_TOP)
			y -= h;

		if (align & TXT_CENTERX)
			x -= (w / 2);

		if (align & TXT_CENTERY)
			y -= (h / 2);
	}

	I::MatSystemSurface->DrawSetTextPos(x, y);
	I::MatSystemSurface->DrawSetTextFont(fnt);
	I::MatSystemSurface->DrawSetTextColor(clr);
	I::MatSystemSurface->DrawPrintText(wstr, V_wcslen(wstr));
}

void CDraw::Line(const int x, const int y, const int x1, const int y1, const Color& clr)
{
	I::MatSystemSurface->DrawSetColor(clr);
	I::MatSystemSurface->DrawLine(x, y, x1, y1);
}

void CDraw::Rect(const int x, const int y, const int w, const int h, const Color& clr)
{
	I::MatSystemSurface->DrawSetColor(clr);
	I::MatSystemSurface->DrawFilledRect(x, y, x + w, y + h);
}

void CDraw::OutlinedRect(const int x, const int y, const int w, const int h, const Color& clr)
{
	I::MatSystemSurface->DrawSetColor(clr);
	I::MatSystemSurface->DrawOutlinedRect(x, y, x + w, y + h);
}

void CDraw::GradientRect(const int x, const int y, const int x1, const int y1, const Color& clrTop, const Color& clrBottom, const bool bHorizontal)
{
	I::MatSystemSurface->DrawSetColor(clrTop);
	I::MatSystemSurface->DrawFilledRectFade(x, y, x1, y1, 255u, 255u, bHorizontal);

	I::MatSystemSurface->DrawSetColor(clrBottom);
	I::MatSystemSurface->DrawFilledRectFade(x, y, x1, y1, 0u, 255u, bHorizontal);
}

void CDraw::CornerRect(const int x, const int y, const int w, const int h, const int _x, const int _y, const Color color)
{
	Line(x, y, x + (w / _x), y, color);
	Line(x, y, x, y + (h / _y), color);

	Line(x + w, y, x + w - (w / _x), y, color);
	Line(x + w, y, x + w, y + (h / _y), color);

	Line(x, y + h, x + (w / _x), y + h, color);
	Line(x, y + h, x, y + h - (h / _y), color);

	Line(x + w, y + h, x + w - (w / _x), y + h, color);
	Line(x + w, y + h, x + w, y + h - (h / _y), color);
}

void CDraw::OutlinedCircle(const int x, const int y, const int radius, const int segments, const Color clr)
{
	I::MatSystemSurface->DrawSetColor(clr);
	I::MatSystemSurface->DrawOutlinedCircle(x, y, radius, segments);
}

void CDraw::Circle(const int x, const int y, const int radius, const int segments, const Color clr)
{
	CUtlVector<vgui::Vertex_t> vecVertixes;

	const float flStep = (6.28318530718f / static_cast<float>(segments));

	for (float n = 0.0f; n < 6.28318530718f; n += flStep)
		vecVertixes.AddToTail(vgui::Vertex_t({ (static_cast<float>(radius) * FastCos(n) + x), (static_cast<float>(radius) * ::sinf(n) + y) }, { 0.0f, 0.0f }));

	if (vecVertixes.Count() > 0)
	{
		I::MatSystemSurface->DrawSetColor(clr);
		I::MatSystemSurface->DrawTexturedPolygon(segments, vecVertixes.Base());
	}
}

void CDraw::ReloadMatrix()
{
	CViewSetup View;
	if (I::BaseClient->GetPlayerView(View))
	{
		static VMatrix WorldToView = { }, ViewToProjection = { }, WorldToPixels = { }, WorldToProjection = { };
		View.ComputeViewMatrices(&WorldToView, &ViewToProjection, &g_WorldToProjection);
	}
}

void CDraw::ReloadScreenSize()
{
	g_Globals.m_nScreenHeight = I::BaseClient->GetScreenHeight();
	g_Globals.m_nScreenWidht = I::BaseClient->GetScreenWidth();
}

int CDraw::GetFontHeight(const EFonts& font)
{
	return m_Fonts.Element(m_Fonts.Find(font)).m_nTall;
}