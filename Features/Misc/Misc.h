#pragma once

#include "../../SDK/SDK.h"

class CFeatures_Misc
{
public:
	void Run(C_TFPlayer* pLocal, CUserCmd* cmd);

private:
	void Bunnyhop(C_TFPlayer* pLocal, CUserCmd* cmd);
	void AntiWarp(C_TFPlayer* pLocal, CUserCmd* cmd);
};

namespace F { inline CFeatures_Misc Misc; }