#ifndef LEGOLUA_H
#define LEGOLUA_H

#include <sol/sol.hpp>

typedef struct {
	bool halted;
	sol::function function;
} LegoLua_CallbackItem;

#endif
