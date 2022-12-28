#pragma once

#include "../Utilities/Size.hpp"
#include "Texture.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <map>
#include <utility>

class TileSet
{
public:
    explicit TileSet(std::string name);
    ~TileSet();
    int getArea() const;
    Size getSize() const;
    SDL_Rect *getCollitionBox(int) const;
    void renderTileClip(int, int, int);

private:
    Texture texture;
    std::string name;
    Size tileSize;
    std::vector<SDL_Rect> tileClips;
    std::map<int, SDL_Rect *> collitions;
};