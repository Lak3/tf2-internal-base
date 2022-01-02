#pragma once

#include "../ConVars/ConVars.h"

#ifdef CreateFont
#undef CreateFont
#endif

enum class EFonts
{
	DEBUG,
	ESP,
	ESP_NAME,
	ESP_WEAPON,
	MENU_TAHOMA,
	MENU_CONSOLAS,
	MENU_VERDANA,
	MENU_ARIAL,
	MENU_TAB,

	FONT_LAST
};

class CDraw
{
private:
	class CFont
	{
	public:
		const char* m_szName;
		int m_nTall;
		int m_nWeight;
		int m_nFlags;
		vgui::HFont m_hFont;
	};

public:
	bool Initialize();
	void UnInitialize();

public:
	void ReloadFonts();
	void ReloadScreenSize();
	void ReloadMatrix();

public:
	void String(const EFonts& font, int x, int y, const Color& clr, const short align, const char* const str, ...);
	void String(const EFonts& font, int x, int y, const Color& clr, const short align, const wchar_t* const str, ...);

	void Line(const int x, const int y, const int x1, const int y1, const Color& clr);
	void Rect(const int x, const int y, const int w, const int h, const Color& clr);
	void OutlinedRect(const int x, const int y, const int w, const int h, const Color& clr);
	void GradientRect(const int x, const int y, const int x1, const int y1, const Color& clrTop, const Color& clrBottom, const bool bHorizontal);
	void CornerRect(const int x, const int y, const int w, const int h, const int _x, const int _y, const Color color);
	void OutlinedCircle(const int x, const int y, const int radius, const int segments, const Color clr);
	void Circle(const int x, const int y, const int radius, const int segments, const Color clr);

public:
	int GetFontHeight(const EFonts& font);

public:
	CUtlMap<EFonts, CFont> m_Fonts;
};

namespace G { inline CDraw Draw; }

#define TXT_DEFAULT		(1 << 0)
#define TXT_LEFT		(1 << 1)
#define TXT_TOP			(1 << 2)
#define TXT_CENTERX		(1 << 3)
#define TXT_CENTERY		(1 << 4)
#define TXT_CENTERXY	TXT_CENTERX | TXT_CENTERY