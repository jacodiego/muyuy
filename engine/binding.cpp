#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "engine/script.hpp"
#include "engine/screen.hpp"
#include "engine/system.hpp"
#include "engine/video/video.hpp"

namespace muyuy::binding
{
    using namespace script;
    using namespace screen;
    using namespace system;
    using namespace video;
    class script::ScriptEngine;
    void bindEngine()
    {
        sol::state &lua = scriptManager->getGlobalState();

        sol::table screen_module = lua["screen"].get_or_create<sol::table>();
        screen_module.new_usertype<GameScreen>("GameScreen",
                                               "getScriptSupervisor", &GameScreen::getScriptSupervisor);

        sol::table video_module = lua["video"].get_or_create<sol::table>();
        video_module.new_usertype<Texture>("Texture",
                                           "draw", sol::overload(static_cast<void (Texture::*)(ScreenPosition, int, int, int, int, float, float)>(&Texture::draw), static_cast<void (Texture::*)(int, int, int, int, int, int, float, float)>(&Texture::draw)),
                                           "getWidth", &Texture::getWidth,
                                           "getHeight", &Texture::getHeight);

        video_module.new_usertype<VideoEngine>("VideoEngine",
                                               "createImage", &VideoEngine::createImage,
                                               "getWindowWidth", &VideoEngine::getWindowWidth,
                                               "getWindowHeight", &VideoEngine::getWindowHeight);

        video_module.new_enum<ScreenPosition>("ScreenPosition", {{"TopLeft", ScreenPosition::TopLeft},
                                                                 {"TopCenter", ScreenPosition::TopCenter},
                                                                 {"TopRight", ScreenPosition::TopRight},
                                                                 {"MiddleLeft", ScreenPosition::MiddleLeft},
                                                                 {"Center", ScreenPosition::Center},
                                                                 {"MiddleRight", ScreenPosition::MiddleRight},
                                                                 {"BottomLeft", ScreenPosition::BottomLeft},
                                                                 {"BottomCenter", ScreenPosition::BottomCenter},
                                                                 {"BottomRight", ScreenPosition::BottomRight}});

        // sol::table script_module = lua["script"].get_or_create<sol::table>();
        // script_module.new_usertype<ScriptSupervisor>("ScriptSupervisor",
        //                                              "createImage", &ScriptSupervisor::createImage);

        sol::table system_module = lua["system"].get_or_create<sol::table>();
        system_module.new_usertype<SystemTimer>("SystemTimer", sol::constructors<SystemTimer(), SystemTimer(uint32_t, uint32_t)>(),
                                                "update", sol::overload(static_cast<void (SystemTimer::*)()>(&SystemTimer::update), static_cast<void (SystemTimer::*)(uint32_t)>(&SystemTimer::update)),
                                                "isRunning", &SystemTimer::isRunning,
                                                "isPaused", &SystemTimer::isPaused,
                                                "isFinished", &SystemTimer::isFinished,
                                                "isInitialized", &SystemTimer::isInitialized,
                                                "run", &SystemTimer::run,
                                                "reset", &SystemTimer::reset,
                                                "finish", &SystemTimer::finish,
                                                "getTimeXpirated", &SystemTimer::getTimeXpirated);

        sol::global_table global = lua.globals();
        global["videoManager"] = videoManager;
    }
}
