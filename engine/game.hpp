#pragma once

#include "utils/singleton.hpp"
#include "utils/ecs.hpp"
#include "engine/screen.hpp"
#include "engine/script.hpp"
#include "engine/video/video.hpp"
#include "screens/map/map_screen.hpp"

#include "components/animation.hpp"
#include "components/character.hpp"
#include "components/movement.hpp"
#include "components/position.hpp"
#include "components/rotation.hpp"
#include "components/sprite.hpp"

#include <entt/entity/registry.hpp>
#include <unordered_map>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace muyuy::game
{
    class GameEngine;
    extern GameEngine *gameManager;

    class GameEngine : public utils::Singleton<GameEngine>
    {
        friend class utils::Singleton<GameEngine>;

    public:
        ~GameEngine();

        bool singletonInitialize() { return true; };
        bool newGame();
        void loadEntities(std::string);
        void setEntities(sol::table);
        entt::registry &getRegistry() { return _registry; };

    private:
        GameEngine();
        script::ScriptReader _game_script;
        entt::registry _registry;
        std::unordered_map<std::string, entt::entity> _global_entities;
    };
}
