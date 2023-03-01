#pragma once

#include "engine/input.hpp"
#include "engine/game.hpp"
#include "engine/screen.hpp"
#include "engine/script.hpp"
#include "engine/system.hpp"
#include "engine/video/video.hpp"
#include "engine/video/color.hpp"

#include "systems/input.hpp"
#include "systems/movement.hpp"
#include "systems/drawer.hpp"

#include "map.hpp"

namespace muyuy::map
{

    enum class MapState
    {
        Explore,
        Dialogue,
        Scene
    };

    class MapScreen : public screen::GameScreen
    {
    public:
        MapScreen(std::string);
        ~MapScreen();
        static MapScreen *currentInstance()
        {
            return _current_instance;
        }
        void reset();
        void update();
        void draw();

    private:
        static MapScreen *_current_instance;
        std::string _map_filename;
        std::string _plot_filename;
        Map _map;
        Rect _camera;
        MapState _map_state;
    };
};