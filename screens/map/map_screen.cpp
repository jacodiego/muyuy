#include "map_screen.hpp"

namespace muyuy::map
{
    MapScreen *MapScreen::_current_instance = nullptr;

    MapScreen::MapScreen(std::string map_filename) : GameScreen(screen::ScreenType::Map), _map(nullptr),
                                                     _camera(Rect{0, 0, 800, 600})
    {
        _current_instance = this;

        loadTilesets();
        _map = new Map(map_filename, &_tilesets);
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
        _map->draw(_camera);
        ecs::systems::Drawer::walkers(game::gameManager->getRegistry(), 0, _camera);
    }

    void MapScreen::loadTilesets()
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);

        lua.script_file("data/tilesets/tilesets.lua");

        sol::table tilesets = lua["tilesets"];

        std::vector<video::TextureMap> texture_map;
        Tileset *tset;

        for (const auto &tileset : tilesets)
        {
            sol::object tileset_key = tileset.first;
            sol::table tileset_value = tileset.second.as<sol::table>();

            lua.script_file(tileset_value["pathname"].get<std::string>());

            sol::table ts = lua["tileset"];

            std::map<int, Rect *> collitions;
            sol::table colls = ts["colliders"].get<sol::table>();

            for (int c = 0; c <= colls.size(); c++)
            {
                collitions.insert(std::make_pair(colls[c]["id"].get<int>(), new Rect(colls[c]["x"].get<int>(),
                                                                                     colls[c]["y"].get<int>(),
                                                                                     colls[c]["width"].get<int>(),
                                                                                     colls[c]["height"].get<int>())));
            }

            tset = new Tileset(
                ts["image"].get<std::string>(),
                ts["columns"].get<int>(),
                ts["rows"].get<int>(),
                ts["tilewidth"].get<int>(),
                ts["tileheight"].get<int>(),
                tileset_value["index"].get<int>(),
                collitions);

            _tilesets.insert(std::make_pair(ts["name"].get<std::string>(), tset));
            texture_map.push_back(video::TextureMap{
                .texture = tset->getTexture(),
                .index = tileset_value["index"].get<int>()});
        }
        video::videoManager->createTextureArray("tilesets", texture_map);
    }
};