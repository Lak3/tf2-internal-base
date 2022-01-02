#pragma once

#include "../../SDK/SDK.h"

class CFeatures_ESP
{
public:
	void Render(C_TFPlayer* pLocal);

private:
	bool GetDynamicBounds(C_BaseEntity* pEntity, int& x, int& y, int& w, int& h);
};

namespace F { inline CFeatures_ESP ESP; }