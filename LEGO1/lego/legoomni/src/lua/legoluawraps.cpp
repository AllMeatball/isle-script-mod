#include "legoluawraps.h"

void LegoAnimationManager_SolWrap(sol::state& p_lua)
{
	p_lua["AnimationManager"] = &AnimationManager;

	sol::usertype<LegoAnimationManager> anim_mgr_type = p_lua.new_usertype<LegoAnimationManager>(
		"LegoAnimationManager",

		"FUN_10061010",
		&LegoAnimationManager::FUN_10061010
	);
}

void LegoWorld_SolWrap(sol::state& p_lua)
{
	sol::usertype<LegoWorld> eventnotif_type = p_lua.new_usertype<LegoWorld>(
		"LegoWorld",

		"Escape",
		&LegoWorld::Escape
	);
}

void LegoEventNotificationParam_SolWrap(sol::state& p_lua)
{
	sol::usertype<LegoEventNotificationParam> eventnotif_type = p_lua.new_usertype<LegoEventNotificationParam>(
		"LegoEventNotificationParam",

		"GetNotification",
		&LegoEventNotificationParam::GetNotification,

		"GetROI",
		&LegoEventNotificationParam::GetROI,

		"GetKey",
		&LegoEventNotificationParam::GetKey,

		"GetX",
		&LegoEventNotificationParam::GetX,

		"GetY",
		&LegoEventNotificationParam::GetY
	);
}

void LegoGameState_SolWrap(sol::state &p_lua)
{
	p_lua["GameState"] = &GameState;
	sol::usertype<LegoBackgroundColor> bgcolor_type = p_lua.new_usertype<LegoBackgroundColor>(
		"LegoBackgroundColor",

		"ToggleDayNight",
		&LegoBackgroundColor::ToggleDayNight,

		"SetLightColor",
		&LegoBackgroundColor::SetLightColor,

		"SetLightColorHSV",
		&LegoBackgroundColor::SetLightColorHSV
	);

	sol::usertype<LegoGameState> gamestate_type = p_lua.new_usertype<LegoGameState>(
		"LegoGameState",

		"GetBackgroundColor",
		&LegoGameState::GetBackgroundColor,

		"Save",
		&LegoGameState::Save
	);
}

void LegoNavController_SolWrap(sol::state &p_lua)
{
	sol::usertype<LegoNavController> navctl_type = p_lua.new_usertype<LegoNavController>(
		"LegoNavController",

		"Unk0x5dHack",
		&LegoNavController::Unk0x5dHack
	);
}

void LegoVideoManager_SolWrap(sol::state &p_lua)
{
	sol::usertype<LegoVideoManager> videomanager_type = p_lua.new_usertype<LegoVideoManager>(
		"LegoVideoManager",
		"EnableFullScreenMovieWithScale",
		&LegoVideoManager::EnableFullScreenMovieWithScale,

		"SetRender3D",
		&LegoVideoManager::SetRender3D,

		"GetRender3D",
		&LegoVideoManager::GetRender3D,

		"SetSkyColor",
		&LegoVideoManager::SetSkyColor,

		"ToggleFPS",
		&LegoVideoManager::ToggleFPS,

		"Get3DManager",
		&LegoVideoManager::Get3DManager,

		"GetRenderer",
		&LegoVideoManager::GetRenderer
	);

	p_lua["VideoManager"] = &VideoManager;
}

void MxBackgroundAudioManager_SolWrap(sol::state &p_lua)
{
	p_lua["BackgroundAudioManager"] = &BackgroundAudioManager;

	sol::usertype<MxBackgroundAudioManager> bgmgr_type = p_lua.new_usertype<MxBackgroundAudioManager>(
		"MxBackgroundAudioManager",

		"Stop",
		&MxBackgroundAudioManager::Stop
	);
}

void MxTransitionManager_SolWrap(sol::state &p_lua)
{
	p_lua["TransitionManager"] = &TransitionManager;

	sol::usertype<MxTransitionManager> bgmgr_type = p_lua.new_usertype<MxTransitionManager>(
		"MxTransitionManager",

		"StartTransition",
		&MxTransitionManager::StartTransition
	);
}

void MxDSAction_SolWrap(sol::state& p_lua)
{
	sol::usertype<MxDSAction> mxdsaction_type = p_lua.new_usertype<MxDSAction>(
		"MxDSAction",
		sol::constructors<MxDSAction()>(),

		"SetAtomId",
		&MxDSAction::SetAtomId,

		"SetUnknown24",
		&MxDSAction::SetUnknown24,

		"SetObjectId",
		&MxDSAction::SetObjectId
	);
}

void MxStreamController_SolWrap(sol::state& p_lua)
{
	sol::usertype<MxStreamController> streamcontroller_type =
	p_lua.new_usertype<MxStreamController>("MxStreamController", "GetAtom", &MxStreamController::GetAtom);
}

void MxStreamer_SolWrap(sol::state& p_lua)
{
	sol::usertype<MxStreamer> streamer_type = p_lua.new_usertype<MxStreamer>("MxStreamer", "Open", &MxStreamer::Open);
	p_lua["Streamer"] = &Streamer; // Create Streamer function
}

void MxTimer_SolWrap(sol::state &p_lua)
{
	p_lua["Timer"] = &Timer;
	sol::usertype<MxTimer> timer_type = p_lua.new_usertype<MxTimer>(
		"MxTimer",
		sol::constructors<MxTimer()>(),

		"GetTime",
		&MxTimer::GetTime,

		"Start",
		&MxTimer::Start,

		"Stop",
		&MxTimer::Stop,

		"GetRealTime",
		&MxTimer::GetRealTime,

		"InitLastTimeCalculated",
		&MxTimer::InitLastTimeCalculated
	);
}

void MxVariableTable_SolWrap(sol::state &p_lua)
{
	p_lua["VariableTable"] = &VariableTable;

	sol::usertype<MxVariableTable> vartable_type = p_lua.new_usertype<MxVariableTable>(
		"MxVariableTable",

		"SetVariable",
		&MxVariableTable::SetVariable,

		"GetVariable",
		&MxVariableTable::GetVariable
	);
}
