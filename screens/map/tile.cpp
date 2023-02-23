#include "tile.hpp"

namespace muyuy::map
{
    Tile::Tile(Coordinate c, Tileset *ts, int t) : tileset(ts), type(t)
    {
        box.x = c.x;
        box.y = c.y;
        box.width = ts->getSize().width;
        box.height = ts->getSize().height;
    }

    video::RenderTile Tile::getRenderTile(Rect &camera) const
    {
        return tileset->getRenderTile(box.x - camera.x, box.y - camera.y, type);
    }

    Rect Tile::getBox() const
    {
        return box;
    }

    Rect *Tile::getCollitionBox() const
    {
        return tileset->getCollitionBox(type);
    }
}