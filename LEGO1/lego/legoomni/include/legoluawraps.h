#ifndef LEGOLUAWRAPS_H
#define LEGOLUAWRAPS_H
#include <sol/sol.hpp>

void LegoEventNotificationParam_SolWrap(sol::state& p_lua);
void LegoGameState_SolWrap(sol::state &p_lua);
void LegoNavController_SolWrap(sol::state &p_lua);

void LegoVideoManager_SolWrap(sol::state &p_lua);

void MxBackgroundAudioManager_SolWrap(sol::state &p_lua);
void MxDSAction_SolWrap(sol::state& p_lua);
void MxTransitionManager_SolWrap(sol::state &p_lua);
void MxTimer_SolWrap(sol::state &p_lua);
void MxVariableTable_SolWrap(sol::state &p_lua);
void MxStreamController_SolWrap(sol::state& p_lua);
void MxStreamer_SolWrap(sol::state& p_lua);
#endif
