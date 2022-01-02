#include "Hooks.h"

bool Hooks::Initialize()
{
	if (MH_Initialize() != MH_STATUS::MH_OK)
		return false;

	BaseEntity::Initialize();
	BasePlayer::Initialize();
	CL_Main::Initialize();
	BaseClient::Initialize();
	ClientEntityList::Initialize();
	ClientMode::Initialize();
	ClientPrediction::Initialize();
	EngineClient::Initialize();
	EngineVGui::Initialize();
	Input::Initialize();
	ModelRender::Initialize();
	NetChannel::Initialize();
	SequenceTransitioner::Initialize();
	TFPlayer::Initialize();
	TFWeaponBase::Initialize();
	VGuiPanel::Initialize();
	ViewRender::Initialize();
	WndProc::Initialize();

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_STATUS::MH_OK)
		return false;

	//If we arrive here, return success.
	return true;
}

bool Hooks::UnInitialize()
{
	WndProc::Uninitialize();

	if (MH_Uninitialize() != MH_STATUS::MH_OK)
		return false;

	//If we arrive here, return success.
	return true;
}