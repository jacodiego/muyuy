#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <map>
#include <string>
#include <vector>
#include "engine/script.hpp"
#include "engine/video/video.hpp"
#include "utils/singleton.hpp"

namespace muyuy::screen
{
    class GameScreen;
}
namespace muyuy::script
{
    class ScriptSupervisor
    {
    public:
        ~ScriptSupervisor();
        void addScript(const std::string &filename);
        void initialize(screen::GameScreen *gs);
        void update();

    private:
        std::vector<std::string> _script_filenames;
        std::vector<sol::function> _reset_functions;
        std::vector<sol::function> _update_functions;
        // std::vector<video::Image *> _images;
    };
}