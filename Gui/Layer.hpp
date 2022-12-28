#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "Tile.hpp"
#include "TileSet.hpp"
#include "../Utilities/Coordinate.hpp"

class Layer
{
public:
    Layer(std::vector<int>, TileSet *, Coordinate);
    ~Layer();
    std::vector<Tile *> getTileMap() const;
    void render(SDL_Rect &) const;
    Tile *getTile(int) const;

private:
    std::vector<Tile *> tileMap;
    std::vector<int> tileTypes;
    TileSet *tileset;
    Coordinate layerSize;
};