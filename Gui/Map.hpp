#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "TileSet.hpp"
#include "Layer.hpp"
#include "../Utilities/Size.hpp"
#include "../Utilities/Coordinate.hpp"
#include "../Utilities/Grid.hpp"

class Map
{
public:
    Map(std::string);
    ~Map();
    std::vector<Layer *> getLayers() const;
    void render(SDL_Rect &camera) const;
    Size getSize() const;
    std::string getName() const;
    bool getColliders(Coordinate) const;
    bool getCollidersGrid(GridLocation) const;

    Size getGridSize() const;
    GridLocation getGridPosition(Coordinate) const;
    SquareGrid getGrid() const;

private:
    std::string name;
    std::vector<Layer *> layers;
    TileSet *tileset;
    Size mapSize;
    Size tileSize;
    SquareGrid grid;
};