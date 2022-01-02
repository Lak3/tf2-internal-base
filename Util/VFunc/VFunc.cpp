#include "VFunc.h"

void**& VTable_Get(void* inst, const unsigned int offset)
{
	return *reinterpret_cast<void***>(reinterpret_cast<unsigned int>(inst) + offset);
}

const void** VTable_Get(const void* inst, const unsigned int offset)
{
	return *reinterpret_cast<const void***>(reinterpret_cast<unsigned int>(inst) + offset);
}