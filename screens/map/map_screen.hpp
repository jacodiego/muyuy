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

#include <unordered_map>

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
        void loadTilesets();

    private:
        static MapScreen *_current_instance;
        Map *_map;
        std::unordered_map<std::string, Tileset *> _tilesets;
        Rect _camera;
        MapState _map_state;
    };
};