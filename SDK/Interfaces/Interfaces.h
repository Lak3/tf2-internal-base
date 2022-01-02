#pragma once

#include "../hl2_src/hl2_src.h"

#define XASSERTFALSE(x) if (x) { Error(#x); return false; }

class CInterfaces
{
public:
	bool Initialize();
};

inline CInterfaces g_Interfaces;