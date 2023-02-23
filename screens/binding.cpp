#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "boot/boot.hpp"
#include "map/map.hpp"

namespace muyuy::binding
{
    class script::ScriptEngine;
    void bindScreen()
    {
        sol::state &lua = script::scriptManager->getGlobalState();

        sol::table boot_module = lua["boot"].get_or_create<sol::table>();
        boot_module.new_usertype<boot::BootScreen>("BootScreen",
                                                   sol::constructors<>(),
                                                   "getState", &boot::BootScreen::getState,
                                                   "changeState", &boot::BootScreen::changeState,
                                                   sol::base_classes, sol::bases<screen::GameScreen>());
        boot_module.new_enum<boot::BootState>("BootState", {{"Intro", boot::BootState::Intro},
                                                            {"Credits", boot::BootState::Credits},
                                                            {"Menu", boot::BootState::Menu}});

        sol::table map_module = lua["map"].get_or_create<sol::table>();
        map_module.new_usertype<map::MapScreen>("MapScreen", sol::constructors<>(),
                                                sol::base_classes, sol::bases<screen::GameScreen>());
    }
}
