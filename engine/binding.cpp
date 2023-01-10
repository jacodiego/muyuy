#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "engine/script.hpp"
#include "engine/screen.hpp"
#include "engine/system.hpp"

namespace muyuy
{
    namespace binding
    {
        using namespace script;
        using namespace screen;
        using namespace system;
        class script::ScriptEngine;
        void bindEngine()
        {
            sol::state &lua = scriptManager->getGlobalState();

            sol::table screen_module = lua["screen"].get_or_create<sol::table>();
            screen_module.new_usertype<GameScreen>("GameScreen",
                                                   "getScriptSupervisor", &GameScreen::getScriptSupervisor);

            sol::table system_module = lua["system"].get_or_create<sol::table>();
            system_module.new_usertype<SystemTimer>("SystemTimer", sol::constructors<SystemTimer(), SystemTimer(uint32_t, uint32_t)>(),
                                                    "update", sol::overload(static_cast<void (SystemTimer::*)()>(&SystemTimer::update), static_cast<void (SystemTimer::*)(uint32_t)>(&SystemTimer::update)),
                                                    "isRunning", &SystemTimer::isRunning,
                                                    "isPaused", &SystemTimer::isPaused,
                                                    "isFinished", &SystemTimer::isFinished,
                                                    "isInitialized", &SystemTimer::isInitialized,
                                                    "run", &SystemTimer::run,
                                                    "reset", &SystemTimer::reset,
                                                    "finish", &SystemTimer::finish);
        }
    }
}
