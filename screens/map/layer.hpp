#pragma once

#include <string>
#include <vector>
#include "tile.hpp"
#include "tileset.hpp"
#include "utils/coordinate.hpp"
#include "engine/video/types.hpp"

namespace muyuy::map
{
    class Layer
    {
    public:
        Layer(std::vector<std::vector<int>> &, std::vector<Tileset *> &, Coordinate);
        ~Layer();
        void draw(Rect &) const;
        Tile *getTile(int) const;
        std::vector<Rect> getColliders() const;

    private:
        std::vector<Tile *> tileMap;
        std::vector<Tileset *> tilesets;
        Coordinate layerSize;
    };
}