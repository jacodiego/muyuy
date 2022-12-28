#pragma once

#include "../Utilities/Size.hpp"
#include "../Gui/SpriteSheet.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <map>
#include <utility>
#include <string>

namespace Graphics
{
    void init();
    void destroy();
    SDL_Renderer *getRenderer();
    void renderClear();
    void renderPresent();
    Size getScreenSize();
    SpriteSheet *getSprite(std::string);
};