#pragma once

#include "utils/singleton.hpp"
#include "engine/screen.hpp"
#include "engine/script.hpp"
#include "screens/map/map_screen.hpp"

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

    private:
        GameEngine();
        script::ScriptReader _game_script;
    };
}
