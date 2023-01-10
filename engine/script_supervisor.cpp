#include "script_supervisor.hpp"
#include "engine/screen.hpp"
#include "screens/boot/boot.hpp"

#include <iostream>

namespace muyuy
{

    ScriptSupervisor::~ScriptSupervisor()
    {
        _reset_functions.clear();
        _update_functions.clear();
    }

    void ScriptSupervisor::addScript(const std::string &filename)
    {
        if (!filename.empty())
            _script_filenames.push_back(filename);
    }

    void ScriptSupervisor::initialize(screen::GameScreen *gs)
    {
        for (uint32_t i = 0; i < _script_filenames.size(); ++i)
        {
            std::string tablespace = script::ScriptEngine::getTableSpace(_script_filenames[i]);
            sol::state &lua = script::scriptManager->getGlobalState();
            lua.open_libraries(sol::lib::base);
            lua.script_file(_script_filenames[i]);
            sol::function init = lua["initialize"];
            sol::function update = lua["update"];
            sol::function reset = lua["reset"];

            if (update.valid())
                _update_functions.push_back(update);
            if (reset.valid())
                _reset_functions.push_back(reset);

            if (init.valid())
            {
                switch (gs->getScreenType())
                {
                case screen::ScreenType::Boot:
                    init((boot::BootScreen *)gs);
                    break;

                default:
                    break;
                }
            }
        }
    }

    void ScriptSupervisor::update()
    {
        for (uint32_t i = 0; i < _update_functions.size(); ++i)
            _update_functions[i]();
    }
}