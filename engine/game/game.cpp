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
        screen::screenManager->push(new map::MapScreen("data/worlds/earth/home.lua"), false, true);
        return _game_script.run("NewGame");
    }
}