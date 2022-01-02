#include "../public/tier1/KeyValues.h"
#include "../public/tier0/platform.h"

#pragma warning (disable : 6031)

bool CKeyValUtils::LoadFromBuffer(KeyValues* key_value, char const* resource_name, const char* buffer, void* file_system, const char* path_id)
{
	return reinterpret_cast<int(__thiscall*)(KeyValues*, char const*, const char*, void*, const char*)>(g_Offsets.m_dwLoadFromBuffer)(key_value, resource_name, buffer, file_system, path_id);
}

KeyValues* CKeyValUtils::Initialize(KeyValues* key_value, char* name)
{
	return reinterpret_cast<KeyValues* (__thiscall*)(KeyValues*, char*)>(g_Offsets.m_dwInitializeKeyValue)(key_value, name);
}