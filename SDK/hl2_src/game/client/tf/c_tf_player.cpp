#include "c_tf_player.h"

#include "../../shared/tf/tf_weaponbase.h"

C_TFWeaponBase* C_TFPlayer::GetActiveTFWeapon()
{
	C_BaseCombatWeapon* pWeapon = GetActiveWeapon();

	return pWeapon ? pWeapon->As<C_TFWeaponBase*>() : nullptr;
}