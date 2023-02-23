#include "map_screen.hpp"

#include <iostream>

namespace muyuy::map
{
    MapScreen *MapScreen::_current_instance = nullptr;

    MapScreen::MapScreen(std::string map_filename) : GameScreen(screen::ScreenType::Map),
                                                     _map_filename(map_filename),
                                                     _map(map_filename),
                                                     _camera(Rect{0, 0, 800, 600})
    {
        _current_instance = this;

        getScriptSupervisor().initialize(this);
        std::cout << "Map Screen init" << std::endl;
    }

    MapScreen::~MapScreen() {}

    void MapScreen::reset()
    {
    }

    void MapScreen::update()
    {
        GameScreen::update();
    }

    void MapScreen::draw()
    {
        getScriptSupervisor().draw();
        _map.draw(_camera);
    }
};