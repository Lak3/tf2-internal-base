//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose: Defines the client-side representation of CBaseCombatCharacter.
//
// $NoKeywords: $
//=============================================================================//
#ifndef C_BASECOMBATCHARACTER_H
#define C_BASECOMBATCHARACTER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseflex.h"
#include "../shared/shareddefs.h"

class C_BaseCombatWeapon;
class C_WeaponCombatShield;

#define BCC_DEFAULT_LOOK_TOWARDS_TOLERANCE 0.9f

class C_BaseCombatCharacter : public C_BaseFlex
{
public:
	enum LineOfSightCheckType {
		IGNORE_NOTHING,
		IGNORE_ACTORS
	};

public:
	virtual						~C_BaseCombatCharacter() = 0;

	virtual bool				IsLookingTowards(const C_BaseEntity* target, float cosTolerance = BCC_DEFAULT_LOOK_TOWARDS_TOLERANCE) const = 0;
	virtual bool				IsLookingTowards(const Vector& target, float cosTolerance = BCC_DEFAULT_LOOK_TOWARDS_TOLERANCE) const = 0;
	virtual bool				IsInFieldOfView(C_BaseEntity* entity) const = 0;
	virtual bool				IsInFieldOfView(const Vector& pos) const = 0;
	virtual bool				IsLineOfSightClear(C_BaseEntity* entity, LineOfSightCheckType checkType = IGNORE_NOTHING) const = 0;
	virtual bool				IsLineOfSightClear(const Vector& pos, LineOfSightCheckType checkType = IGNORE_NOTHING, C_BaseEntity* entityToIgnore = NULL) const = 0;
	virtual	bool				Weapon_Switch(C_BaseCombatWeapon* pWeapon, int viewmodelindex = 0) = 0;
	virtual bool				Weapon_CanSwitchTo(C_BaseCombatWeapon* pWeapon) = 0;
	virtual C_BaseCombatWeapon* GetActiveWeapon(void) const = 0;
	virtual void				GetGlowEffectColor(float* r, float* g, float* b) = 0;
	virtual void				UpdateGlowEffect(void) = 0;
	virtual void				DestroyGlowEffect(void) = 0;

public:
	int							WeaponCount() const;
	C_BaseCombatWeapon*			GetWeapon(const int i);

public:
	M_NETVAR(m_flNextAttack, float, "CBaseCombatCharacter", "m_flNextAttack");
	M_NETVAR(m_hActiveWeapon, CHandle<C_BaseCombatWeapon>, "CBaseCombatCharacter", "m_hActiveWeapon");
	M_NETVAR(m_bGlowEnabled, bool, "CBaseCombatCharacter", "m_bGlowEnabled");

public:
	inline std::array<CHandle<C_BaseCombatWeapon>, MAX_WEAPONS>& m_hMyWeapons()
	{
		static const int nOffset = GetNetVar("CBaseCombatCharacter", "m_hMyWeapons");
		return *reinterpret_cast<std::array<CHandle<C_BaseCombatWeapon>, MAX_WEAPONS>*>(reinterpret_cast<DWORD>(this) + nOffset);
	}

	inline bool CanAttack()
	{
		return (I::GlobalVars->curtime >= m_flNextAttack());
	}
};

inline int C_BaseCombatCharacter::WeaponCount() const
{
	return MAX_WEAPONS;
}

inline C_BaseCombatWeapon* C_BaseCombatCharacter::GetWeapon(const int i)
{
	assert((i >= 0) && (i < MAX_WEAPONS));
	return m_hMyWeapons()[i].Get();
}

#endif //C_BASECOMBATCHARACTER_H