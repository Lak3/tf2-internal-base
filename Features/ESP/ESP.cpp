#include "ESP.h"

#include "../Vars.h"

void CFeatures_ESP::Render(C_TFPlayer* pLocal)
{
	if (!Vars::ESP::Enabled)
		return;

	int x, y, w, h;
	for (int n = 1; n < (g_Globals.m_nMaxEntities + 1); n++)
	{
		if (n == g_Globals.m_nLocalIndex)
			continue;

		IClientEntity* pEntity = UTIL_EntityByIndex(n);

		if (!pEntity || pEntity->IsDormant())
			continue;

		ClientClass* pCC = pEntity->GetClientClass();

		if (!pCC)
			continue;

		switch (pCC->GetTFClientClass())
		{
			case ETFClientClass::CTFPlayer:
			{
				if (!Vars::ESP::Players::Enabled)
					break;

				C_TFPlayer* pPlayer = pEntity->As<C_TFPlayer*>();

				if (pPlayer->deadflag() || (Vars::ESP::Players::IgnoreTeam && pPlayer->InLocalTeam()))
					break;

				if (!GetDynamicBounds(pPlayer, x, y, w, h))
					break;

				const int nHealth = pPlayer->GetHealth();
				const int nMaxHealth = pPlayer->GetMaxHealth();

				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				const Color clrTeam = G::Util.GetTeamColor(pPlayer->GetTeamNumber());

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);

				player_info_t pi;
				if (Vars::ESP::Players::Name && I::EngineClient->GetPlayerInfo(n, &pi))
				{
					uchar16 szName[MAX_PLAYER_NAME_LENGTH];
					Q_UTF8ToUTF16(UTIL_SafeName(pi.name), szName, MAX_PLAYER_NAME_LENGTH);

					G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, clrTeam, TXT_CENTERXY, szName);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
				}

				if (Vars::ESP::Players::HealthText)
				{
					G::Draw.String(EFonts::ESP, nDrawX, nDrawY, clrHealth, TXT_CENTERXY, L"%i / %ihp", nHealth, nMaxHealth);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP);
				}

				if (Vars::ESP::Players::ActiveWeapon)
				{
					C_BaseCombatWeapon* pWeapon = pPlayer->GetActiveWeapon();

					if (pWeapon)
					{
						G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, COLOR_GREY, TXT_CENTERXY, pWeapon->GetName() + 10); //tf_weapon_
						nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);
					}
				}

				break;
			}
			case ETFClientClass::CObjectSentrygun:
			{
				if (!Vars::ESP::Buildings::Enabled)
					break;

				C_ObjectSentrygun* pSentry = pEntity->As<C_ObjectSentrygun*>();

				if (Vars::ESP::Buildings::IgnoreTeam && pSentry->InLocalTeam())
					break;
				
				if (!GetDynamicBounds(pSentry, x, y, w, h))
					break;

				const int nHealth = pSentry->GetHealth();
				const int nMaxHealth = pSentry->GetMaxHealth();

				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				const Color clrTeam = G::Util.GetTeamColor(pSentry->GetTeamNumber());

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);

				if (Vars::ESP::Buildings::Name)
				{
					G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, clrTeam, TXT_CENTERXY, L"sentrygun");
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
				}

				if (Vars::ESP::Buildings::HealthText)
				{
					G::Draw.String(EFonts::ESP, nDrawX, nDrawY, clrHealth, TXT_CENTERXY, L"%i / %ihp", nHealth, nMaxHealth);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP);
				}

				if (Vars::ESP::Buildings::Info)
				{
					int nShells, nMaxShells, nRockets, nMaxRockets;
					pSentry->GetAmmoCount(nShells, nMaxShells, nRockets, nMaxRockets);
 
					G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, COLOR_GREY, TXT_CENTERXY, L"shells: %i / %i", nShells, nMaxShells);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);

					if (nMaxRockets)
					{
						G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, COLOR_GREY, TXT_CENTERXY, L"rockets: %i / %i", nRockets, nMaxRockets);
						nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);
					}
				}

				break;
			}
			case ETFClientClass::CObjectDispenser:
			{
				if (!Vars::ESP::Buildings::Enabled)
					break;

				C_ObjectDispenser* pDispenser = pEntity->As<C_ObjectDispenser*>();

				if (Vars::ESP::Buildings::IgnoreTeam && pDispenser->InLocalTeam())
					break;

				if (!GetDynamicBounds(pDispenser, x, y, w, h))
					break;

				const int nHealth = pDispenser->GetHealth();
				const int nMaxHealth = pDispenser->GetMaxHealth();

				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				const Color clrTeam = G::Util.GetTeamColor(pDispenser->GetTeamNumber());

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);

				if (Vars::ESP::Buildings::Name)
				{
					G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, clrTeam, TXT_CENTERXY, L"dispenser");
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
				}

				if (Vars::ESP::Buildings::HealthText)
				{
					G::Draw.String(EFonts::ESP, nDrawX, nDrawY, clrHealth, TXT_CENTERXY, L"%i / %ihp", nHealth, nMaxHealth);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP);
				}

				if (Vars::ESP::Buildings::Info)
				{
					G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, COLOR_GREY, TXT_CENTERXY, L"metal: %i / %i", pDispenser->m_iAmmoMetal(), pDispenser->GetMaxMetal());
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);
				}

				break;
			}
			case ETFClientClass::CObjectTeleporter:
			{
				if (!Vars::ESP::Buildings::Enabled)
					break;

				C_ObjectTeleporter* pTeleport = pEntity->As<C_ObjectTeleporter*>();

				if (Vars::ESP::Buildings::IgnoreTeam && pTeleport->InLocalTeam())
					break;

				if (!GetDynamicBounds(pTeleport, x, y, w, h))
					break;

				const int nHealth = pTeleport->GetHealth();
				const int nMaxHealth = pTeleport->GetMaxHealth();

				const Color clrHealth = G::Util.GetHealthColor(nHealth, nMaxHealth);
				const Color clrTeam = G::Util.GetTeamColor(pTeleport->GetTeamNumber());

				const int nDrawX = x + (w / 2);
				int nDrawY = y + (h / 2);

				if (Vars::ESP::Buildings::Name)
				{
					G::Draw.String(EFonts::ESP_NAME, nDrawX, nDrawY, clrTeam, TXT_CENTERXY, L"teleport");
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP_NAME);
				}

				if (Vars::ESP::Buildings::HealthText)
				{
					G::Draw.String(EFonts::ESP, nDrawX, nDrawY, clrHealth, TXT_CENTERXY, L"%i / %ihp", nHealth, nMaxHealth);
					nDrawY += G::Draw.GetFontHeight(EFonts::ESP);
				}

				if (Vars::ESP::Buildings::Info)
				{
					const float flChargeTime = pTeleport->GetChargeTime();

					if (flChargeTime > 0.0f)
					{
						G::Draw.String(EFonts::ESP_WEAPON, nDrawX, nDrawY, COLOR_GREY, TXT_CENTERXY, L"charging: %.1f", flChargeTime);
						nDrawY += G::Draw.GetFontHeight(EFonts::ESP_WEAPON);
					}
				}

				break;
			}
			default:
				break;
		}
	}
}

bool CFeatures_ESP::GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h)
{
	Vector vPoints[8];
	BuildTransformedBox(vPoints, pEntity->m_vecMins(), pEntity->m_vecMaxs(), pEntity->RenderableToWorldTransform());

	Vector2D flb, brt, blb, frt, frb, brb, blt, flt;

	if (GetVectorInScreenSpace(vPoints[3], flb) && GetVectorInScreenSpace(vPoints[5], brt)
		&& GetVectorInScreenSpace(vPoints[0], blb) && GetVectorInScreenSpace(vPoints[4], frt)
		&& GetVectorInScreenSpace(vPoints[2], frb) && GetVectorInScreenSpace(vPoints[1], brb)
		&& GetVectorInScreenSpace(vPoints[6], blt) && GetVectorInScreenSpace(vPoints[7], flt)
		&& GetVectorInScreenSpace(vPoints[6], blt) && GetVectorInScreenSpace(vPoints[7], flt))
	{
		const Vector2D arr[8] = { flb, brt, blb, frt, frb, brb, blt, flt };

		float left = flb.x;
		float top = flb.y;
		float righ = flb.x;
		float bottom = flb.y;

		for (int n = 1; n < 8; n++)
		{
			if (left > arr[n].x)
				left = arr[n].x;

			if (top < arr[n].y)
				top = arr[n].y;

			if (righ < arr[n].x)
				righ = arr[n].x;

			if (bottom > arr[n].y)
				bottom = arr[n].y;
		}

		x = static_cast<int>(left);
		y = static_cast<int>(bottom);
		w = static_cast<int>((righ - left));
		h = static_cast<int>((top - bottom));

		return !(x > g_Globals.m_nScreenWidht || (x + w) < 0 || y > g_Globals.m_nScreenHeight || (y + h) < 0);
	}

	return false;
}