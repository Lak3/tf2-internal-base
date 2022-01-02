#pragma once

#include "../../SDK/SDK.h"

class CFeatures_Prediction
{
public:
	void Start(C_TFPlayer* pLocal, CUserCmd* cmd);
	void Finish(C_TFPlayer* pLocal);

private:
	int GetTickbase(C_TFPlayer* pLocal, CUserCmd* pCmd);

private:
	float m_flOldCurTime = 0.0f;
	float m_flOldFrameTime = 0.0f;

	int m_nOldTickCount = 0;

	bool m_bOldInPrediction = false;
	bool m_bOldFirstTimePredicted = false;

	CUtlFlags<int> m_nOldFlags = { };

	CMoveData m_MoveData = { };
};

namespace F { inline CFeatures_Prediction Prediction; }