#include "map_screen.hpp"

namespace muyuy::map
{
    MapScreen *MapScreen::_current_instance = nullptr;

    MapScreen::MapScreen(const std::string &map_filename, const std::string &script_filename) : GameScreen(screen::ScreenType::Map), _map(nullptr),
                                                                                                _camera(Rect{0, 0, 800, 600}), _script_filename(script_filename)
    {
        _current_instance = this;

        _loadTilesets();
        _map = new Map(map_filename, &_tilesets);
        _load();
        ecs::systems::Camera::followCharacter(game::gameManager->getRegistry(), _camera, _map);
        getScriptSupervisor().initialize(this);
    }

    MapScreen::~MapScreen() {}

    void MapScreen::reset()
    {
    }

    void MapScreen::update()
    {
        GameScreen::update();
        ecs::systems::Controller::move(game::gameManager->getRegistry());
        ecs::systems::Animator::walkers(game::gameManager->getRegistry());
        ecs::systems::Animator::objects(_map_registry);
        ecs::systems::Interaction::open(game::gameManager->getRegistry(), _map_registry);
        ecs::systems::Movement::character(game::gameManager->getRegistry(), _map_registry, _camera, _map);
        ecs::systems::Camera::followCharacter(game::gameManager->getRegistry(), _camera, _map);
    }

    void MapScreen::draw()
    {
        getScriptSupervisor().draw();
        _map->draw(_camera);
        ecs::systems::Renderer::walkers(game::gameManager->getRegistry(), _camera);
        ecs::systems::Renderer::npc(_map_registry, _camera);
        ecs::systems::Renderer::objects(_map_registry, _camera);
        ecs::systems::Renderer::openables(_map_registry, _camera);
    }

    void MapScreen::createObject(sol::table obj, int x, int y)
    {
        auto object = _map_registry.create();
        for (const auto &o : obj)
        {
            if (o.first.as<std::string>() == "sprite")
                _map_registry.emplace<ecs::components::Sprite>(object,
                                                               o.second.as<sol::table>().get<uint16_t>("width"),
                                                               o.second.as<sol::table>().get<uint16_t>("height"),
                                                               o.second.as<sol::table>().get<uint16_t>("rows"),
                                                               o.second.as<sol::table>().get<uint16_t>("cols"),
                                                               video::videoManager->createImage(o.second.as<sol::table>().get<std::string>("image_filename").c_str()));
            if (o.first.as<std::string>() == "position")
                _map_registry.emplace<ecs::components::Position>(object, x, y);

            if (o.first.as<std::string>() == "collisionable")
                _map_registry.emplace<ecs::components::Collisionable>(object);

            if (o.first.as<std::string>() == "openable")
                _map_registry.emplace<ecs::components::Openable>(object);

            if (o.first.as<std::string>() == "animation")
            {
                std::unordered_map<std::string, std::vector<std::pair<uint16_t, uint16_t>>> state_map;
                std::vector<std::pair<uint16_t, uint16_t>> frames;
                for (const auto &frame : o.second.as<sol::table>())
                {
                    sol::table frame_value = frame.second.as<sol::table>();
                    frames.push_back(std::make_pair(frame_value.get<uint16_t>("id"), frame_value.get<uint16_t>("duration")));
                }
                state_map.insert(std::make_pair("animation", frames));
                _map_registry.emplace<ecs::components::Animation>(object, state_map);
            }
        }
    }

    void MapScreen::loadEntities(sol::table entities)
    {
        for (const auto &o : entities)
        {
            auto ent = _map_registry.create();
            utils::updateEntity(&_map_registry, ent, o.second.as<sol::table>());
        }
    }

    void MapScreen::_load()
    {
        sol::state &lua = script::scriptManager->getGlobalState();
        lua.require_file("objects", "data/entities/objects.lua");
        lua.script_file(_script_filename);
        sol::function fx = lua["load"];
        fx(this);
    }

    void MapScreen::_loadTilesets()
    {
        sol::state &lua = script::scriptManager->getGlobalState();

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