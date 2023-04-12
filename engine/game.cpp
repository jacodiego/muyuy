#include "game.hpp"

namespace muyuy::game
{

    GameEngine *gameManager = nullptr;

    GameEngine::GameEngine() : _game_script(script::ScriptReader{"data/game.lua"})
    {
    }

    GameEngine::~GameEngine()
    {
    }

    bool GameEngine::newGame()
    {
        screen::screenManager->pop(false, false);
        video::videoManager->clearScreenTextures();
        loadEntities("characters");
        screen::screenManager->push(new map::MapScreen("data/worlds/earth/home.lua", "data/plot/episode1/home1.lua"), false, true);
        return _game_script.run("NewGame");
    }

    void GameEngine::loadEntities(std::string type)
    {
        sol::state &lua = script::scriptManager->getGlobalState();

        lua.script_file("data/entities/" + type + ".lua");

        sol::table entities = lua[type];

        for (const auto &entity : entities)
        {
            sol::object entity_key = entity.first;
            sol::table entity_value = entity.second.as<sol::table>();

            if (_global_entities.find(entity_key.as<std::string>()) != _global_entities.end())
            {
                std::cerr << "Entity already exist" << std::endl;
                continue;
            }
            _global_entities.insert(std::make_pair(entity_key.as<std::string>(), _registry.create()));

            for (const auto &components : entity_value)
            {
                sol::object component_key = components.first;
                sol::table component_value = components.second.as<sol::table>();

                if (component_key.as<std::string>() == "character")
                    _registry.emplace<ecs::components::Character>(_global_entities.at(entity_key.as<std::string>()), component_value.get<std::string>("name"));
                if (component_key.as<std::string>() == "sprite")
                    _registry.emplace<ecs::components::Sprite>(_global_entities.at(entity_key.as<std::string>()),
                                                               component_value.get<uint16_t>("width"),
                                                               component_value.get<uint16_t>("height"),
                                                               component_value.get<uint16_t>("rows"),
                                                               component_value.get<uint16_t>("cols"),
                                                               video::videoManager->createImage(component_value.get<std::string>("image_filename").c_str()));
                if (component_key.as<std::string>() == "movement")
                    _registry.emplace<ecs::components::Movement>(_global_entities.at(entity_key.as<std::string>()));
                if (component_key.as<std::string>() == "position")
                    _registry.emplace<ecs::components::Position>(_global_entities.at(entity_key.as<std::string>()));
                if (component_key.as<std::string>() == "rotation")
                    _registry.emplace<ecs::components::Rotation>(_global_entities.at(entity_key.as<std::string>()));

                if (component_key.as<std::string>() == "animation")
                {
                    std::unordered_map<std::string, std::vector<std::pair<uint16_t, uint16_t>>> state_map;
                    for (const auto &properties : component_value)
                    {
                        sol::object property_key = properties.first;
                        sol::table property_value = properties.second.as<sol::table>();
                        std::vector<std::pair<uint16_t, uint16_t>> frames;

                        for (const auto &frame : property_value)
                        {
                            sol::table frame_value = frame.second.as<sol::table>();
                            frames.push_back(std::make_pair(frame_value.get<uint16_t>("id"), frame_value.get<uint16_t>("duration")));
                        }
                        state_map.insert(std::make_pair(property_key.as<std::string>(), frames));
                    }
                    _registry.emplace<ecs::components::Animation>(_global_entities.at(entity_key.as<std::string>()), state_map);
                }
            }
        }
    }
}