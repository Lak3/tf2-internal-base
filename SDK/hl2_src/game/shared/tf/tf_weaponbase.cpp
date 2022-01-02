#pragma once

#include "tf_weaponbase.h"
#include "../../client/tf/c_tf_player.h"

bool C_TFWeaponBase::CanFireCiriticalShotHelper(const bool bHeadShot)
{
	bool bRet = false;

	C_BasePlayer* pOwner = m_hOwner().Get()->As<C_BasePlayer*>();

	if (pOwner)
	{
		const int nOldFov = pOwner->m_iFOV();

		pOwner->m_iFOV() = 70;
		bRet = VFunc_Get<bool(__thiscall*)(void*, bool, C_BaseEntity*)>(this, 423u)(this, bHeadShot, nullptr);
		pOwner->m_iFOV() = nOldFov;
	}

	return bRet;
}