//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $Workfile:     $
// $Date:         $
// $NoKeywords: $
//=============================================================================//
#if !defined( CLIENTMODE_NORMAL_H )
#define CLIENTMODE_NORMAL_H
#ifdef _WIN32
#pragma once
#endif

#include "iclientmode.h"
#include "hud_basechat.h"
#include "../shared/GameEventListener.h"

class CBaseHudWeaponSelection;
class CViewSetup;
class C_BaseEntity;
class C_BasePlayer;
class CBaseViewport;

namespace vgui
{
	class Panel;
}

//=============================================================================
// HPE_BEGIN:
// [tj] Moved this from the .cpp file so derived classes could access it
//=============================================================================
#define ACHIEVEMENT_ANNOUNCEMENT_MIN_TIME 10

//=============================================================================
// HPE_END
//=============================================================================

#define USERID2PLAYER(i) I::ClientEntityList->GetClientEntity(I::EngineClient->GetPlayerForUserID(i))->As<C_BasePlayer*>();

// This class implements client mode functionality common to HL2 and TF2.
class ClientModeShared : public IClientMode, public CGameEventListener
{
private:
	char __pad00[27]; //28

public:
	// Message mode handling
	// All modes share a common chat interface
	CBaseHudChat*            m_pChatElement;
	vgui::HCursor			 m_CursorNone;
	CBaseHudWeaponSelection* m_pWeaponSelection;
	int						 m_nRootSize[2];
};

namespace I { inline ClientModeShared* ClientMode = nullptr; }

#endif // CLIENTMODE_NORMAL_H