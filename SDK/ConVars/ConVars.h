#pragma once

#include "../Interfaces/Interfaces.h"

class CConVars
{
public:
	bool Initialize();

public:
	ConVar* cl_interp = nullptr;
	ConVar* cl_interp_ratio = nullptr;
	ConVar* cl_updaterate = nullptr;
};

namespace G { inline CConVars ConVars; }