#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include "Timer.hpp"
#include "../Utilities/Size.hpp"
#include "Graphics.hpp"
#include "SceneManager.hpp"

class Game
{
public:
    Game();
    ~Game();

    void run();
    static void close() { isRunning = false; }

private:
    void init(const char *title, Size size, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    SDL_Event event;
    inline static bool isRunning = false;
    const int FRAME_RATE = 60;
};