#include "Tile.hpp"

Tile::Tile(Coordinate c, TileSet *ts, int t) : tileset(ts), type(t)
{
    box.x = c.x;
    box.y = c.y;
    box.w = ts->getSize().w;
    box.h = ts->getSize().h;
    // tileset = ts;
    // type = t;
}

void Tile::render(SDL_Rect &camera) const
{
    tileset->renderTileClip(box.x - camera.x, box.y - camera.y, type);
}

SDL_Rect Tile::getBox() const
{
    return box;
}

SDL_Rect *Tile::getCollitionBox() const
{
    return tileset->getCollitionBox(type);
}