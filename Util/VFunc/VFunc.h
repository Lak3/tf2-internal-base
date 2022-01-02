#pragma once

void**& VTable_Get(void* inst, const unsigned int offset = 0u);
const void** VTable_Get(const void* inst, const unsigned int offset = 0u);

template<typename T>
inline T VFunc_Get(const void* inst, const unsigned int index, const unsigned int offset = 0u)
{
	return reinterpret_cast<T>(VTable_Get(inst, offset)[index]);
}