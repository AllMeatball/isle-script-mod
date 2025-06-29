#include "legolua.h"

#include <SDL3/SDL.h>

#include "misc.h"
#include "compat.h"
#include "legoeventnotificationparam.h"

// (For single character delimiter) by Quonux and Arafat Hasan https://stackoverflow.com/a/46931770 under CC BY-SA 4.0
std::vector<std::string> stringSplit(const std::string &s, char delim) {
	std::vector<std::string> result;
	std::stringstream ss (s);
	std::string item;

	while (getline(ss, item, delim)) {
		result.push_back(item);
	}

	return result;
}

// (For single character delimiter) by Quonux and Arafat Hasan https://stackoverflow.com/a/46931770 under CC BY-SA 4.0*
// *modifed into another function
std::string stringReplaceChar(const std::string &s, char delim, std::string replacement) {
	std::string result;

	for (char c : s) {
		if (c == delim) {
			result += replacement;
			continue;
		}

		result += c;
	}

	return result;
}

MxString LegoLua_GetScriptPath(const char* p_path)
{
	// TODO: maybe use physfs to avoid the weird pathing
	sol::state &lua = Lego()->m_lua;
	std::string pkg_path = lua["package"]["path"];

	std::vector<std::string> pkg_paths = stringSplit(pkg_path, ';');

	for (std::string pkg_path : pkg_paths) {
		std::string script_path = stringReplaceChar(pkg_path, '?', p_path);

		MxString full_script_path(Lego()->GetHD());
		full_script_path += "/lua/";
		full_script_path += script_path.c_str();

		full_script_path.MapPathToFilesystem();

		if (!SDL_GetPathInfo(full_script_path.GetData(), nullptr))
			continue;

		// SDL_Log("%s: %s", pkg_path.c_str(), full_script_path.GetData());

		return full_script_path;
	}

	return "";
}

bool LegoLua_API_ShowMessageBox(int flags, const char* message)
{
	return Any_ShowSimpleMessageBox(flags, "LEGO® Island (Lua)", message, nullptr);
}

int LegoLua_Loader(lua_State* L)
{
	std::string path = sol::stack::get<std::string>(L, 1);

	MxString script_path = LegoLua_GetScriptPath(path.c_str());

	if (luaL_loadfile(L, script_path.GetData()) != LUA_OK) {
		return 1;
	}

	return 1;
}

void LegoLua_LoadEnums(sol::state &p_lua)
{
	p_lua["MESSAGEBOX_ERROR"] = SDL_MESSAGEBOX_ERROR;
	p_lua["MESSAGEBOX_WARNING"] = SDL_MESSAGEBOX_WARNING;
	p_lua["MESSAGEBOX_INFORMATION"] = SDL_MESSAGEBOX_INFORMATION;
	p_lua["MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT"] = SDL_MESSAGEBOX_BUTTONS_LEFT_TO_RIGHT;
	p_lua["MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT"] = SDL_MESSAGEBOX_BUTTONS_RIGHT_TO_LEFT;

	p_lua.new_enum(
		"WorldId",

		"e_undefined",
		LegoOmni::World::e_undefined,

		"e_act1",
		LegoOmni::World::e_act1,

		"e_imain",
		LegoOmni::World::e_imain,

		"e_icube",
		LegoOmni::World::e_icube,

		"e_ireg",
		LegoOmni::World::e_ireg,

		"e_ielev",
		LegoOmni::World::e_ielev,

		"e_iisle",
		LegoOmni::World::e_iisle,

		"e_hosp",
		LegoOmni::World::e_hosp,

		"e_police",
		LegoOmni::World::e_police,

		"e_gmain",
		LegoOmni::World::e_gmain,

		"e_bldh",
		LegoOmni::World::e_bldh,

		"e_bldd",
		LegoOmni::World::e_bldd,

		"e_bldj",
		LegoOmni::World::e_bldj,

		"e_bldr",
		LegoOmni::World::e_bldr,

		"e_racc",
		LegoOmni::World::e_racc,

		"e_racj",
		LegoOmni::World::e_racj,

		"e_act2",
		LegoOmni::World::e_act2,

		"e_act3",
		LegoOmni::World::e_act3,

		"e_test",
		LegoOmni::World::e_test,

		"e_numWorlds",
		LegoOmni::World::e_numWorlds
	);


	p_lua.new_enum(
		"LegoEventNotificationButton",

		"c_lButtonState",
		LegoEventNotificationParam::c_lButtonState,

		"c_rButtonState",
		LegoEventNotificationParam::c_rButtonState,

		"c_modKey1",
		LegoEventNotificationParam::c_modKey1,

		"c_modKey2",
		LegoEventNotificationParam::c_modKey2
	);

	p_lua.new_enum(
		"NotificationId",

		"c_notificationType0",
		NotificationId::c_notificationType0,

		"c_notificationStartAction",
		NotificationId::c_notificationStartAction,

		"c_notificationEndAction",
		NotificationId::c_notificationEndAction,

		"c_notificationType4",
		NotificationId::c_notificationType4,

		"c_notificationPresenter",
		NotificationId::c_notificationPresenter,

		"c_notificationStreamer",
		NotificationId::c_notificationStreamer,

		"c_notificationKeyPress",
		NotificationId::c_notificationKeyPress,

		"c_notificationButtonUp",
		NotificationId::c_notificationButtonUp,

		"c_notificationButtonDown",
		NotificationId::c_notificationButtonDown,

		"c_notificationMouseMove",
		NotificationId::c_notificationMouseMove,

		"c_notificationClick",
		NotificationId::c_notificationClick,

		"c_notificationDragStart",
		NotificationId::c_notificationDragStart,

		"c_notificationDrag",
		NotificationId::c_notificationDrag,

		"c_notificationDragEnd",
		NotificationId::c_notificationDragEnd,

		"c_notificationTimer",
		NotificationId::c_notificationTimer,

		"c_notificationControl",
		NotificationId::c_notificationControl,

		"c_notificationEndAnim",
		NotificationId::c_notificationEndAnim,

		"c_notificationPathStruct",
		NotificationId::c_notificationPathStruct,

		"c_notificationType20",
		NotificationId::c_notificationType20,

		"c_notificationNewPresenter",
		NotificationId::c_notificationNewPresenter,

		"c_notificationType22",
		NotificationId::c_notificationType22,

		"c_notificationType23",
		NotificationId::c_notificationType23,

		"c_notificationTransitioned",
		NotificationId::c_notificationTransitioned
	);

	p_lua.new_enum(
		"ActionType",

		"e_none",
		Extra::ActionType::e_none,

		"e_opendisk",
		Extra::ActionType::e_opendisk,

		"e_openram",
		Extra::ActionType::e_openram,

		"e_close",
		Extra::ActionType::e_close,

		"e_start",
		Extra::ActionType::e_start,

		"e_stop",
		Extra::ActionType::e_stop,

		"e_run",
		Extra::ActionType::e_run,

		"e_exit",
		Extra::ActionType::e_exit,

		"e_enable",
		Extra::ActionType::e_enable,

		"e_disable",
		Extra::ActionType::e_disable,

		"e_notify",
		Extra::ActionType::e_notify,

		"e_unknown",
		Extra::ActionType::e_unknown
	);

	p_lua.new_enum(
		"TransitionType",

		"e_idle",
		MxTransitionManager::e_idle,

		"e_noAnimation",
		MxTransitionManager::e_noAnimation,

		"e_dissolve",
		MxTransitionManager::e_dissolve,

		"e_mosaic",
		MxTransitionManager::e_mosaic,

		"e_wipeDown",
		MxTransitionManager::e_wipeDown,

		"e_windows",
		MxTransitionManager::e_windows,

		"e_broken",
		MxTransitionManager::e_broken
	);
}

void LegoLua_BindScripts() {
	sol::state &lua = Lego()->m_lua;

	lua["g_copterScript"]   = g_copterScript;
	lua["g_dunecarScript"]  = g_dunecarScript;
	lua["g_jetskiScript"]   = g_jetskiScript;
	lua["g_racecarScript"]  = g_racecarScript;
	lua["g_carraceScript"]  = g_carraceScript;
	lua["g_carracerScript"] = g_carracerScript;
	lua["g_jetraceScript"]  = g_jetraceScript;
	lua["g_jetracerScript"] = g_jetracerScript;
	lua["g_isleScript"]     = g_isleScript;
	lua["g_elevbottScript"] = g_elevbottScript;
	lua["g_infodoorScript"] = g_infodoorScript;
	lua["g_infomainScript"] = g_infomainScript;
	lua["g_infoscorScript"] = g_infoscorScript;
	lua["g_regbookScript"]  = g_regbookScript;
	lua["g_histbookScript"] = g_histbookScript;
	lua["g_hospitalScript"] = g_hospitalScript;
	lua["g_policeScript"]   = g_policeScript;
	lua["g_garageScript"]   = g_garageScript;
	lua["g_act2mainScript"] = g_act2mainScript;
	lua["g_act3Script"]     = g_act3Script;
	lua["g_jukeboxScript"]  = g_jukeboxScript;
	lua["g_pz5Script"]      = g_pz5Script;
	lua["g_introScript"]    = g_introScript;
	lua["g_testScript"]     = g_testScript;
	lua["g_jukeboxwScript"] = g_jukeboxwScript;
	lua["g_sndAnimScript"]  = g_sndAnimScript;
	lua["g_creditsScript"]  = g_creditsScript;
	lua["g_nocdSourceName"] = g_nocdSourceName;
}
