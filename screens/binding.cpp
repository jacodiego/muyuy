#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "boot/boot.hpp"

namespace muyuy
{
    namespace binding
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
        }
    }
}
