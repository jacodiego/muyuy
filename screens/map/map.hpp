#pragma once

#include "layer.hpp"
#include "tileset.hpp"
#include "utils/grid.hpp"

namespace muyuy::map
{
    class Map
    {
    public:
        Map(std::string);
        ~Map();
        std::vector<Layer *> getLayers() const;
        void draw(Rect &) const;
        Size getSize() const;
        std::string getName() const;
        bool checkCollision(Rect) const;
        bool getCollidersGrid(GridLocation) const;

        Size getGridSize() const;
        // GridLocation getGridPosition(Coordinate) const;
        SquareGrid getGrid() const;

    private:
        std::string name;
        std::vector<Layer *> layers;
        std::vector<Tileset *> tilesets;
        Size mapSize;
        Size tileSize;
        SquareGrid grid;
    };
};