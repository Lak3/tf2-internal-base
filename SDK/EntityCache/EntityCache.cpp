#include "EntityCache.h"

void CEntityCache::Fill()
{
	if (m_pLocal = UTIL_TFPlayerByIndex(g_Globals.m_nLocalIndex))
	{
		int nLocalTeam = m_pLocal->m_iTeamNum();

		if (nLocalTeam != TF_TEAM_RED && nLocalTeam != TF_TEAM_BLUE) {
			m_pLocal = nullptr;
			return;
		}

		m_pWeapon = m_pLocal->GetActiveTFWeapon();

		for (int n = 1; n < g_Globals.m_nMaxEntities; n++)
		{
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
				int nPlayerTeam = pEntity->As<C_TFPlayer*>()->m_iTeamNum();

				if (nPlayerTeam != TF_TEAM_RED && nPlayerTeam != TF_TEAM_BLUE)
					continue;

				m_Groups[EEntGroup::PLAYERS_ALL].push_back(pEntity);
				m_Groups[nLocalTeam != nPlayerTeam ? EEntGroup::PLAYERS_ENEMIES : EEntGroup::PLAYERS_TEAMMATES].push_back(pEntity);

				break;
			}

			case ETFClientClass::CObjectSentrygun:
			case ETFClientClass::CObjectDispenser:
			case ETFClientClass::CObjectTeleporter:
			{
				int nObjectTeam = pEntity->As<C_BaseObject*>()->m_iTeamNum();

				m_Groups[EEntGroup::BUILDINGS_ALL].push_back(pEntity);
				m_Groups[nLocalTeam != nObjectTeam ? EEntGroup::BUILDINGS_ENEMIES : EEntGroup::BUILDINGS_TEAMMATES].push_back(pEntity);

				break;
			}

			default: break;
			}
		}
	}
}

void CEntityCache::Clear()
{
	for (auto& Group : m_Groups)
		Group.second.clear();

	m_pLocal = nullptr;
	m_pWeapon = nullptr;
}

const std::vector<IClientEntity*>& CEntityCache::GetGroup(EEntGroup Group) {
	return m_Groups[Group];
}

C_TFPlayer* CEntityCache::GetLocal() {
	return m_pLocal;
}

C_TFWeaponBase* CEntityCache::GetWeapon() {
	return m_pWeapon;
}
