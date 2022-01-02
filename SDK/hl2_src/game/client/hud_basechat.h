//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef HUD_BASECHAT_H
#define HUD_BASECHAT_H
#ifdef _WIN32
#pragma once
#endif

#include "hudelement.h"

class CBaseHudChatInputLine;
class CBaseHudChatEntry;
class CHudChatFilterPanel;

namespace vgui
{
	class IScheme;
};

#define CHATLINE_NUM_FLASHES 8.0f
#define CHATLINE_FLASH_TIME 5.0f
#define CHATLINE_FADE_TIME 1.0f

#define CHAT_HISTORY_FADE_TIME 0.25f
#define CHAT_HISTORY_IDLE_TIME 15.0f
#define CHAT_HISTORY_IDLE_FADE_TIME 2.5f
#define CHAT_HISTORY_ALPHA 127

enum ChatFilters
{
	CHAT_FILTER_NONE = 0,
	CHAT_FILTER_JOINLEAVE = 0x000001,
	CHAT_FILTER_NAMECHANGE = 0x000002,
	CHAT_FILTER_PUBLICCHAT = 0x000004,
	CHAT_FILTER_SERVERMSG = 0x000008,
	CHAT_FILTER_TEAMCHANGE = 0x000010,
	//=============================================================================
	// HPE_BEGIN:
	// [tj]Added a filter for achievement announce
	//=============================================================================

	CHAT_FILTER_ACHIEVEMENT = 0x000020,

	//=============================================================================
	// HPE_END
	//=============================================================================
};

//-----------------------------------------------------------------------------
enum TextColor
{
	COLOR_NORMAL = 1,
	COLOR_USEOLDCOLORS = 2,
	COLOR_PLAYERNAME = 3,
	COLOR_LOCATION = 4,
	COLOR_ACHIEVEMENT = 5,
	COLOR_CUSTOM = 6,		// Will use the most recently SetCustomColor()
	COLOR_HEXCODE = 7,		// Reads the color from the next six characters
	COLOR_HEXCODE_ALPHA = 8,// Reads the color and alpha from the next eight characters
	COLOR_MAX
};

//--------------------------------------------------------------------------------------------------------------
struct TextRange
{
	TextRange() { preserveAlpha = false; }
	int start;
	int end;
	Color color;
	bool preserveAlpha;
};

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
class CBaseHudChat //: public CHudElement, public vgui::EditablePanel
{
public:
	inline void Printf(const int iFilter, const char* const fmt, ...)
	{
		VFunc_Get<void(__thiscall*)(void*, int, const char*)>(this, 18u)(this, iFilter, fmt);
	}

	inline void ChatPrintf(const int iPlayerIndex, const int iFilter, const char* const fmt, ...)
	{
		VFunc_Get<void(__thiscall*)(void*, int, int, const char*)>(this, 19u)(this, iPlayerIndex, iFilter, fmt);
	}

	inline void StartMessageMode(const int iMessageModeType)
	{
		VFunc_Get<void(__thiscall*)(void*, int)>(this, 20u)(this, iMessageModeType);
	}

	inline void StopMessageMode()
	{
		VFunc_Get<void(__thiscall*)(void*)>(this, 21u)(this);
	}
};

#endif // HUD_BASECHAT_H