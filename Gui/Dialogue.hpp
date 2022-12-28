#pragma once

#include "../Core/Graphics.hpp"
#include "../Core/Timer.hpp"
#include "Texture.hpp"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

class Dialogue
{
public:
    Dialogue();
    ~Dialogue();
    void draw(std::string);

private:
    Texture dialogueTexture;
    TTF_Font *font;
};