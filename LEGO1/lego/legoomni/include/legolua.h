#ifndef LEGOLUA_H
#define LEGOLUA_H

#include "mxstring.h"
#include <sol/sol.hpp>

typedef struct {
	bool halted;
	sol::function function;
} LegoLua_CallbackItem;

MxString LegoLua_GetScriptPath(const char* p_path);
bool LegoLua_API_ShowMessageBox(int flags, const char* message);
int LegoLua_Loader(lua_State* L);

void LegoLua_LoadEnums(sol::state &p_lua);
void LegoLua_BindScripts();

#endif
