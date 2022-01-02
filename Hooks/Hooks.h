#pragma once

#include "BaseEntity/BaseEntity.h"
#include "BasePlayer/BasePlayer.h"
#include "CL_Main/CL_Main.h"
#include "Client/Client.h"
#include "ClientEntityList/ClientEntityList.h"
#include "ClientMode/ClientMode.h"
#include "ClientPrediction/ClientPrediction.h"
#include "EngineClient/EngineClient.h"
#include "EngineVGui/EngineVGui.h"
#include "Input/HInput.h"
#include "ModelRender/ModelRender.h"
#include "NetChannel/NetChannel.h"
#include "SequenceTransitioner/SequenceTransitioner.h"
#include "TFWeaponBase/TFWeaponBase.h"
#include "TFPlayer/TFPlayer.h"
#include "VGuiPanel/VGuiPanel.h"
#include "ViewRender/ViewRender.h"
#include "WndProc/WndProc.h"

namespace Hooks
{
	bool Initialize();
	bool UnInitialize();
}