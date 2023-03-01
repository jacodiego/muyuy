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
        screen::screenManager->push(new map::MapScreen("data/worlds/earth/home.lua"), false, true);
        return _game_script.run("NewGame");
    }

    void GameEngine::loadEntities(std::string type)
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);

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
                if (component_key.as<std::string>() == "move")
                    _registry.emplace<ecs::components::Move>(_global_entities.at(entity_key.as<std::string>()), component_value.get<int>("velocity"));
                if (component_key.as<std::string>() == "position")
                    _registry.emplace<ecs::components::Position>(_global_entities.at(entity_key.as<std::string>()));
                if (component_key.as<std::string>() == "state")
                    _registry.emplace<ecs::components::State>(_global_entities.at(entity_key.as<std::string>()));

                for (const auto &properties : component_value)
                {
                    sol::object property_key = properties.first;

                    if (property_key.as<std::string>() == "animations")
                    {
                        sol::table property_value = properties.second.as<sol::table>();
                        for (const auto &animations : property_value)
                        {
                            sol::object animation_key = animations.first;
                            sol::object animation_value = animations.second;
                            video::videoManager->addAnimation(entity_key.as<std::string>() + "_" + animation_key.as<std::string>(), animation_value.as<std::string>());
                        }
                    }
                }
            }
        }
    }
}