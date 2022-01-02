#include "Interfaces.h"

bool CInterfaces::Initialize()
{
	I::BaseClient = reinterpret_cast<IBaseClientDLL*>(CreateInterface("client.dll", CLIENT_DLL_INTERFACE_VERSION));
	XASSERTFALSE(I::BaseClient == nullptr);

	I::ClientDLLSharedAppSystems = reinterpret_cast<IClientDLLSharedAppSystems*>(CreateInterface("client.dll", CLIENT_DLL_SHARED_APPSYSTEMS));
	XASSERTFALSE(I::ClientDLLSharedAppSystems == nullptr);

	I::ClientTools = reinterpret_cast<IClientTools*>(CreateInterface("client.dll", VCLIENTTOOLS_INTERFACE_VERSION));
	XASSERTFALSE(I::ClientTools == nullptr);

	I::ClientPrediction = reinterpret_cast<CPrediction*>(CreateInterface("client.dll", VCLIENT_PREDICTION_INTERFACE_VERSION));
	XASSERTFALSE(I::ClientPrediction == nullptr);

	I::GameMovement = reinterpret_cast<IGameMovement*>(CreateInterface("client.dll", INTERFACENAME_GAMEMOVEMENT));
	XASSERTFALSE(I::GameMovement == nullptr);

	I::ClientLeafSystemEngine = reinterpret_cast<IClientLeafSystemEngine*>(CreateInterface("client.dll", CLIENTLEAFSYSTEM_INTERFACE_VERSION));
	XASSERTFALSE(I::ClientLeafSystemEngine == nullptr);

	I::ClientEntityList = reinterpret_cast<IClientEntityList*>(CreateInterface("client.dll", VCLIENTENTITYLIST_INTERFACE_VERSION));
	XASSERTFALSE(I::ClientEntityList == nullptr);
	
	I::EngineVGui = reinterpret_cast<IEngineVGui*>(CreateInterface("engine.dll", VENGINE_VGUI_VERSION));
	XASSERTFALSE(I::EngineVGui == nullptr);

	I::ClientEngineTools = reinterpret_cast<IClientEngineTools*>(CreateInterface("engine.dll", VCLIENTENGINETOOLS_INTERFACE_VERSION));
	XASSERTFALSE(I::ClientEngineTools == nullptr);

	I::EngineToolFramework = reinterpret_cast<IEngineToolFramework*>(CreateInterface("engine.dll", VENGINETOOLFRAMEWORK_INTERFACE_VERSION));
	XASSERTFALSE(I::EngineToolFramework == nullptr);

	I::EngineTool = reinterpret_cast<IEngineTool*>(CreateInterface("engine.dll", VENGINETOOL_INTERFACE_VERSION));
	XASSERTFALSE(I::EngineTool == nullptr);

	I::GameUIFuncs = reinterpret_cast<IGameUIFuncs*>(CreateInterface("engine.dll", VENGINE_GAMEUIFUNCS_VERSION));
	XASSERTFALSE(I::GameUIFuncs == nullptr);

	I::EngineAPI = reinterpret_cast<IEngineAPI*>(CreateInterface("engine.dll", VENGINE_LAUNCHER_API_VERSION));
	XASSERTFALSE(I::EngineAPI == nullptr);

	I::StaticPropMgrClient = reinterpret_cast<IStaticPropMgrClient*>(CreateInterface("engine.dll", INTERFACEVERSION_STATICPROPMGR_CLIENT));
	XASSERTFALSE(I::StaticPropMgrClient == nullptr);

	I::UniformRandomStream = reinterpret_cast<IUniformRandomStream*>(CreateInterface("engine.dll", VENGINE_RANDOM_INTERFACEVERSION));
	XASSERTFALSE(I::UniformRandomStream == nullptr);

	I::ModelInfoClient = reinterpret_cast<IVModelInfoClient*>(CreateInterface("engine.dll", VMODELINFO_CLIENT_INTERFACE_VERSION));
	XASSERTFALSE(I::ModelInfoClient == nullptr);

	I::MatchMaking = reinterpret_cast<IMatchmaking*>(CreateInterface("engine.dll", VENGINE_MATCHMAKING_VERSION));
	XASSERTFALSE(I::MatchMaking == nullptr);

	I::GameEventManager = reinterpret_cast<IGameEventManager2*>(CreateInterface("engine.dll", INTERFACEVERSION_GAMEEVENTSMANAGER2));
	XASSERTFALSE(I::GameEventManager == nullptr);

	I::EngineTrace = reinterpret_cast<IEngineTrace*>(CreateInterface("engine.dll", INTERFACEVERSION_ENGINETRACE_CLIENT));
	XASSERTFALSE(I::EngineTrace == nullptr);

	I::CvarQuery = reinterpret_cast<ICvarQuery*>(CreateInterface("engine.dll", CVAR_QUERY_INTERFACE_VERSION));
	XASSERTFALSE(I::CvarQuery == nullptr);

	I::GameServerData = reinterpret_cast<IGameServerData*>(CreateInterface("engine.dll", GAMESERVERDATA_INTERFACE_VERSION));
	XASSERTFALSE(I::GameServerData == nullptr);

	I::RenderView = reinterpret_cast<IVRenderView*>(CreateInterface("engine.dll", VENGINE_RENDERVIEW_INTERFACE_VERSION));
	XASSERTFALSE(I::RenderView == nullptr);

	I::ShadowMgr = reinterpret_cast<IShadowMgr*>(CreateInterface("engine.dll", ENGINE_SHADOWMGR_INTERFACE_VERSION));
	XASSERTFALSE(I::ShadowMgr == nullptr);

	I::EngineEffects = reinterpret_cast<CVEfx*>(CreateInterface("engine.dll", VENGINE_EFFECTS_INTERFACE_VERSION));
	XASSERTFALSE(I::EngineEffects == nullptr);

	I::ModelRender = reinterpret_cast<IVModelRender*>(CreateInterface("engine.dll", VENGINE_HUDMODEL_INTERFACE_VERSION));
	XASSERTFALSE(I::ModelRender == nullptr);

	I::PhysicsDebugOverlay = reinterpret_cast<IVPhysicsDebugOverlay*>(CreateInterface("engine.dll", VPHYSICS_DEBUG_OVERLAY_INTERFACE_VERSION));
	XASSERTFALSE(I::PhysicsDebugOverlay == nullptr);

	I::DebugOverlay = reinterpret_cast<IVDebugOverlay*>(CreateInterface("engine.dll", VDEBUG_OVERLAY_INTERFACE_VERSION));
	XASSERTFALSE(I::DebugOverlay == nullptr);

	I::EngineClient = reinterpret_cast<IVEngineClient*>(CreateInterface("engine.dll", VENGINE_CLIENT_INTERFACE_VERSION));
	XASSERTFALSE(I::EngineClient == nullptr);

	I::EngineSound = reinterpret_cast<IEngineSound*>(CreateInterface("engine.dll", IENGINESOUND_CLIENT_INTERFACE_VERSION));
	XASSERTFALSE(I::EngineSound == nullptr);

	I::Cvar = reinterpret_cast<ICvar*>(CreateInterface("vstdlib.dll", CVAR_INTERFACE_VERSION));
	XASSERTFALSE(I::Cvar == nullptr);

	I::InputSystem = reinterpret_cast<IInputSystem*>(CreateInterface("inputsystem.dll", INPUTSYSTEM_INTERFACE_VERSION));
	XASSERTFALSE(I::InputSystem == nullptr);

	I::MaterialSystemHardwareConfig = reinterpret_cast<IMaterialSystemHardwareConfig*>(CreateInterface("materialsystem.dll", MATERIALSYSTEM_HARDWARECONFIG_INTERFACE_VERSION));
	XASSERTFALSE(I::MaterialSystemHardwareConfig == nullptr);

	I::MaterialSystem = reinterpret_cast<IMaterialSystem*>(CreateInterface("materialsystem.dll", MATERIAL_SYSTEM_INTERFACE_VERSION));
	XASSERTFALSE(I::MaterialSystem == nullptr);

	I::ColorCorrectionSystem = reinterpret_cast<IColorCorrectionSystem*>(CreateInterface("materialsystem.dll", COLORCORRECTION_INTERFACE_VERSION));
	XASSERTFALSE(I::ColorCorrectionSystem == nullptr);

	I::StudioDataCache = reinterpret_cast<IStudioDataCache*>(CreateInterface("datacache.dll", STUDIO_DATA_CACHE_INTERFACE_VERSION));
	XASSERTFALSE(I::StudioDataCache == nullptr);

	I::MDLCache = reinterpret_cast<IMDLCache*>(CreateInterface("datacache.dll", MDLCACHE_INTERFACE_VERSION));
	XASSERTFALSE(I::MDLCache == nullptr);

	I::DataCache = reinterpret_cast<IDataCache*>(CreateInterface("datacache.dll", DATACACHE_INTERFACE_VERSION));
	XASSERTFALSE(I::DataCache == nullptr);

	I::StudioRender = reinterpret_cast<IStudioRender*>(CreateInterface("studiorender.dll", STUDIO_RENDER_INTERFACE_VERSION));
	XASSERTFALSE(I::StudioRender == nullptr);

	I::PhysicsSurfaceProps = reinterpret_cast<IPhysicsSurfaceProps*>(CreateInterface("vphysics.dll", VPHYSICS_SURFACEPROPS_INTERFACE_VERSION));
	XASSERTFALSE(I::PhysicsSurfaceProps == nullptr);

	I::PhysicsCollision = reinterpret_cast<IPhysicsCollision*>(CreateInterface("vphysics.dll", VPHYSICS_COLLISION_INTERFACE_VERSION));
	XASSERTFALSE(I::PhysicsCollision == nullptr);

	I::Physics = reinterpret_cast<IPhysics*>(CreateInterface("vphysics.dll", VPHYSICS_INTERFACE_VERSION));
	XASSERTFALSE(I::Physics == nullptr);

	I::VGuiPanel = reinterpret_cast<vgui::IPanel*>(CreateInterface("vgui2.dll", VGUI_PANEL_INTERFACE_VERSION));
	XASSERTFALSE(I::VGuiPanel == nullptr);

	I::MatSystemSurface = reinterpret_cast<IMatSystemSurface*>(CreateInterface("vguimatsurface.dll", MAT_SYSTEM_SURFACE_INTERFACE_VERSION));
	XASSERTFALSE(I::MatSystemSurface == nullptr);

	I::VGui = reinterpret_cast<vgui::IVGui*>(CreateInterface("vgui2.dll", VGUI_IVGUI_INTERFACE_VERSION));
	XASSERTFALSE(I::VGui == nullptr);

	I::VGuiSystem = reinterpret_cast<vgui::ISystem*>(CreateInterface("vgui2.dll", VGUI_SYSTEM_INTERFACE_VERSION));
	XASSERTFALSE(I::VGuiSystem == nullptr);

	I::VGuiSurface = reinterpret_cast<vgui::ISurface*>(CreateInterface("vgui2.dll", VGUI_SURFACE_INTERFACE_VERSION));
	XASSERTFALSE(I::VGuiSurface == nullptr);

	I::VGuiScheme = reinterpret_cast<vgui::IScheme*>(CreateInterface("vgui2.dll", VGUI_SCHEME_INTERFACE_VERSION));
	XASSERTFALSE(I::VGuiScheme == nullptr);

	I::VGuiInputInternal = reinterpret_cast<vgui::IInputInternal*>(CreateInterface("vgui2.dll", VGUI_INPUTINTERNAL_INTERFACE_VERSION));
	XASSERTFALSE(I::VGuiInputInternal == nullptr);

	I::VGuiInput = reinterpret_cast<vgui::IInput*>(CreateInterface("vgui2.dll", VGUI_INPUT_INTERFACE_VERSION));
	XASSERTFALSE(I::VGuiInput == nullptr);

	I::UnicodeWindows = reinterpret_cast<IUnicodeWindows*>(CreateInterface("unicode.dll", VENGINE_UNICODEINTERFACE_VERSION));
	XASSERTFALSE(I::UnicodeWindows == nullptr);

	I::GameUI = reinterpret_cast<IGameUI*>(CreateInterface("GameUI.dll", GAMEUI_INTERFACE_VERSION));
	XASSERTFALSE(I::GameUI == nullptr);

	I::VGuiModuleLoader = reinterpret_cast<IVGuiModuleLoader*>(CreateInterface("GameUI.dll", VGUIMODULELOADER_INTERFACE_VERSION));
	XASSERTFALSE(I::VGuiModuleLoader == nullptr);

	I::GameConsole = reinterpret_cast<IGameConsole*>(CreateInterface("GameUI.dll", GAMECONSOLE_INTERFACE_VERSION));
	XASSERTFALSE(I::GameConsole == nullptr);

	I::RunGameEngine = reinterpret_cast<IRunGameEngine*>(CreateInterface("GameUI.dll", RUNGAMEENGINE_INTERFACE_VERSION));
	XASSERTFALSE(I::RunGameEngine == nullptr);

	I::GlobalVars = *reinterpret_cast<CGlobalVarsBase**>(g_Offsets.m_dwGlobalVars);
	XASSERTFALSE(I::GlobalVars == nullptr);

	I::ClientState = *reinterpret_cast<CClientState**>(g_Offsets.m_dwClientState);
	XASSERTFALSE(I::ClientState == nullptr);

	I::PredictionRandomSeed = *reinterpret_cast<int32**>(g_Offsets.m_dwPredictionRandomSeed);
	XASSERTFALSE(I::PredictionRandomSeed == nullptr);

	{
		const PDWORD pdwClient = reinterpret_cast<PDWORD>(I::BaseClient);
		XASSERTFALSE(pdwClient == nullptr);

		const PDWORD pdwTable = *reinterpret_cast<PDWORD*>(pdwClient);
		XASSERTFALSE(pdwTable == nullptr);

		I::ClientMode = **reinterpret_cast<ClientModeShared***>(static_cast<DWORD>(pdwTable[0xA]) + 0x05);
		XASSERTFALSE(I::ClientMode == nullptr);

		I::ViewRender = **reinterpret_cast<IViewRender***>(static_cast<DWORD>(pdwTable[0x1B]) + 0x05);
		XASSERTFALSE(I::ViewRender == nullptr);

		I::Input = **reinterpret_cast<CInput***>(static_cast<DWORD>(pdwTable[0xE] + 0x02));
		XASSERTFALSE(I::Input == nullptr);
	}

	//If we arrive here, return success.
	return true;
}