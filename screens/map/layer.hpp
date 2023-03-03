#pragma once

#include <string>
#include <vector>
#include "tile.hpp"
#include "tileset.hpp"
#include "utils/coordinate.hpp"
#include "engine/video/types.hpp"

namespace muyuy::map
{

    enum class LayerTypes
    {
        UnderGround,
        Ground,
        OverGround,
        Sky
    };

    class Layer
    {
    public:
        Layer(std::vector<std::vector<int>> &, std::vector<MapTileset> &, Coordinate);
        ~Layer();
        std::vector<video::RenderTile> getRenderTiles(Rect &) const;
        Tile *getTile(int) const;
        std::vector<Rect> getColliders() const;

    private:
        std::vector<Tile *> tileMap;
        std::vector<MapTileset> tilesets;
        Coordinate layerSize;
    };
}