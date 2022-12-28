#pragma once

#include "../Utilities/Coordinate.hpp"
#include "TileSet.hpp"
#include <SDL2/SDL.h>

class Tile
{
public:
    Tile(Coordinate, TileSet *, int);
    void render(SDL_Rect &) const;
    SDL_Rect getBox() const;
    SDL_Rect *getCollitionBox() const;

private:
    SDL_Rect box;
    TileSet *tileset;
    int type;
};