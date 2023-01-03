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
    Layer(std::vector<std::vector<int>> &, std::vector<TileSet *> &, Coordinate);
    ~Layer();
    void render(SDL_Rect &) const;
    Tile *getTile(int) const;
    std::vector<SDL_Rect> getColliders() const;

private:
    std::vector<Tile *> tileMap;
    std::vector<TileSet *> tilesets;
    Coordinate layerSize;
};