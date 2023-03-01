#include "map_screen.hpp"

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
    }

    MapScreen::~MapScreen() {}

    void MapScreen::reset()
    {
    }

    void MapScreen::update()
    {
        GameScreen::update();
        ecs::systems::Input::move(game::gameManager->getRegistry());
        ecs::systems::Movement::character(game::gameManager->getRegistry(), _camera);
    }

    void MapScreen::draw()
    {
        getScriptSupervisor().draw();
        _map.draw(_camera);
        ecs::systems::Drawer::walkers(game::gameManager->getRegistry(), 0, _camera);
    }
};