#include "Prediction.h"

void CFeatures_Prediction::Start(C_TFPlayer* pLocal, CUserCmd* cmd)
{
	if (!I::MoveHelper)
		return;

	pLocal->SaveData("pol_pred_data", 1337, 0);

	memset(&m_MoveData, 0, sizeof(CMoveData));

	pLocal->m_pCurrentCommand() = cmd;
	C_BaseEntity::SetPredictionRandomSeed((MD5_PseudoRandom(cmd->command_number) & INT_MAX));
	//TODO: SetPredictionPlayer, UpdateButtonState

	m_flOldCurTime = I::GlobalVars->curtime;
	m_flOldFrameTime = I::GlobalVars->frametime;
	m_nOldTickCount = I::GlobalVars->tickcount;

	m_bOldFirstTimePredicted = I::ClientPrediction->m_bFirstTimePredicted;
	m_bOldInPrediction = I::ClientPrediction->m_bInPrediction;

	m_nOldFlags = pLocal->m_fFlags();

	const int nTickBase = GetTickbase(pLocal, cmd);

	I::GlobalVars->curtime = TICKS_TO_TIME(nTickBase);
	I::GlobalVars->frametime = I::ClientPrediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL;
	I::GlobalVars->tickcount = nTickBase;

	I::ClientPrediction->m_bFirstTimePredicted = false;
	I::ClientPrediction->m_bInPrediction = true;

	I::GameMovement->StartTrackPredictionErrors(pLocal);

	if (cmd->weaponselect != 0)
	{
		C_BaseCombatWeapon* pWeapon = UTIL_EntityAs<C_BaseCombatWeapon*>(cmd->weaponselect);

		if (pWeapon)
		{
			pLocal->SelectItem(pWeapon->GetName(), cmd->weaponsubtype);
		}
	}

	//TODO: m_nImpulse

	I::ClientPrediction->SetLocalViewAngles(cmd->viewangles);

	if (pLocal->PhysicsRunThink())
		pLocal->PreThink();

	const int nThinkTick = pLocal->m_nNextThinkTick();

	if ((nThinkTick > 0) && (nThinkTick < nTickBase))
	{
		pLocal->m_nNextThinkTick() = TICK_NEVER_THINK;
		pLocal->Think();
	}

	I::ClientPrediction->SetupMove(pLocal, cmd, I::MoveHelper, &m_MoveData);
	I::GameMovement->ProcessMovement(pLocal, &m_MoveData);
	I::ClientPrediction->FinishMove(pLocal, cmd, &m_MoveData);
}

void CFeatures_Prediction::Finish(C_TFPlayer* pLocal)
{
	if (!I::MoveHelper)
		return;

	I::GameMovement->FinishTrackPredictionErrors(pLocal);

	pLocal->m_pCurrentCommand() = nullptr;
	C_BaseEntity::SetPredictionRandomSeed(-1);
	//TODO: SetPredictionPlayer

	I::GlobalVars->curtime = m_flOldCurTime;
	I::GlobalVars->frametime = m_flOldFrameTime;
	I::GlobalVars->tickcount = m_nOldTickCount;

	I::ClientPrediction->m_bFirstTimePredicted = m_bOldFirstTimePredicted;
	I::ClientPrediction->m_bInPrediction = m_bOldInPrediction;

	pLocal->m_fFlags() = m_nOldFlags;

	pLocal->RestoreData("pol_pred_data", 1337, 0);
}

int CFeatures_Prediction::GetTickbase(C_TFPlayer* pLocal, CUserCmd* pCmd)
{
	static int s_nTick = 0;
	static CUserCmd* s_pLastCmd = nullptr;

	if (pCmd)
	{
		if (!s_pLastCmd || s_pLastCmd->hasbeenpredicted)
			s_nTick = pLocal->m_nTickBase();
		else 
			s_nTick++;

		s_pLastCmd = pCmd;
	}

	return s_nTick;
}