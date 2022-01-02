#pragma once

#include "../Globals/Globals.h"
#include "../../Util/Util.h"

class RecvTable;

int GetNetVar(const char* const szClass, const char* const szVar);
int GetOffset(RecvTable* pTable, const char* const szVar);

#define M_NETVAR(_name, type, table, name) inline type &_name() \
{ \
	static const int nOff = GetNetVar(table, name); \
	return *reinterpret_cast<type*>(reinterpret_cast<unsigned long>(this) + nOff); \
}