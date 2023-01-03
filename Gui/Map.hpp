#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "TileSet.hpp"
#include "Layer.hpp"
#include "../Core/Util.hpp"
#include "../Utilities/Size.hpp"
#include "../Utilities/Coordinate.hpp"
#include "../Utilities/Grid.hpp"

class Map
{
public:
    Map(std::string);
    ~Map();
    std::vector<Layer *> getLayers() const;
    void render(SDL_Rect &) const;
    Size getSize() const;
    std::string getName() const;
    bool checkCollision(SDL_Rect) const;
    bool getCollidersGrid(GridLocation) const;

    Size getGridSize() const;
    GridLocation getGridPosition(Coordinate) const;
    SquareGrid getGrid() const;

private:
    std::string name;
    std::vector<Layer *> layers;
    std::vector<TileSet *> tilesets;
    Size mapSize;
    Size tileSize;
    SquareGrid grid;
};