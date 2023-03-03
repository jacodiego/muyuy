#pragma once

#include "layer.hpp"
#include "tileset.hpp"
#include "utils/grid.hpp"

#define TILESETS_COUNT = 2

namespace muyuy::map
{

    class Map
    {
    public:
        Map(std::string, std::unordered_map<std::string, Tileset *> *);
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
        std::string _name;
        std::vector<Layer *> _layers;
        std::vector<MapTileset> _map_tilesets;
        std::unordered_map<std::string, Tileset *> *_tilesets;
        Size _map_size;
        Size _tile_size;
        SquareGrid _grid;
    };
};