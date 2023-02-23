#pragma once

#include "tileset.hpp"
#include "utils/coordinate.hpp"
#include "utils/rect.hpp"

namespace muyuy::map
{
    class Tile
    {
    public:
        Tile(Coordinate, Tileset *, int);
        video::RenderTile getRenderTile(Rect &) const;
        Rect getBox() const;
        Rect *getCollitionBox() const;

    private:
        Rect box;
        Tileset *tileset;
        int type;
    };

}