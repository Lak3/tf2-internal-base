#pragma once

#include "../Hooks/Hooks.h"

class CModuleEntry
{
public:
	void Load();
	void Unload();

public:
	bool ShouldUnload();

private:
	bool m_bShouldUnload = false;
};

inline CModuleEntry g_ModuleEntry;