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
            auto ent = _registry.create();
            _global_entities.insert(std::make_pair(entity_key.as<std::string>(), ent));

            utils::updateEntity(&_registry, ent, entity_value);
        }
    }

    void GameEngine::setEntities(sol::table entities)
    {
        for (const auto &entity : entities)
        {
            sol::object entity_key = entity.first;
            sol::table entity_value = entity.second.as<sol::table>();
            utils::updateEntity(&_registry, _global_entities.at(entity_key.as<std::string>()), entity_value);
        }
    }
}