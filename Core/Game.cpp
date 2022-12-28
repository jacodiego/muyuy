#include "Game.hpp"

using namespace std;

Game::Game()
{
    Graphics::init();
    SceneManager::init();
    Timer::init();
    isRunning = true;
}

Game::~Game()
{
}

void Game::run()
{

    while (isRunning)
    {
        Timer::update();
        handleEvents();
        if (Timer::deltaTime() >= (1.0f / FRAME_RATE))
        {
            update();
            render();
            Timer::reset();
        }
    }
    clean();
}

void Game::handleEvents()
{

    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        close();
        break;
    default:
        SceneManager::processInput(&event);
        break;
    }
}

void Game::update()
{
    SceneManager::update();
}

void Game::render()
{
    Graphics::renderClear();
    SceneManager::draw();
    Graphics::renderPresent();
}

void Game::clean()
{
    SceneManager::destroy();
    Graphics::destroy();
    TTF_Quit();
    SDL_Quit();
}