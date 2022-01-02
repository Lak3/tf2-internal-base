//========= Copyright (c) Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//
#ifndef C_ENTITY_CURRENCYPACK_H
#define C_ENTITY_CURRENCYPACK_H
#ifdef _WIN32
#pragma once
#endif

#include "../c_baseanimating.h"

class C_CurrencyPack : public C_BaseAnimating
{
public:
	M_NETVAR(m_bDistributed, bool, "CCurrencyPack", "m_bDistributed");
};

#endif //C_ENTITY_CURRENCYPACK_H