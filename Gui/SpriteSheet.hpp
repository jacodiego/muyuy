#pragma once

#include "Texture.hpp"
#include "../Utilities/Size.hpp"
#include "../Utilities/Coordinate.hpp"
#include <vector>
#include <string>
#include <SDL2/SDL.h>

class SpriteSheet
{
public:
    SpriteSheet(std::string name, Size size, int count, int cols);
    void renderSpriteClip(Coordinate c, int t, SDL_Rect &camera);
    Size getSize() const;

private:
    Texture texture;
    std::string name;
    Size spriteSize;
    std::vector<SDL_Rect> spriteClips;
};