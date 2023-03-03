#include "tileset.hpp"

namespace muyuy::map
{
    Tileset::Tileset(std::string img, int cols, int rows, int tWidth, int tHeight, int i, std::map<int, Rect *> colls)
        : columns(cols), rows(rows), tileSize({tWidth, tHeight}), index(i), collitions(colls)
    {
        texture = video::videoManager->createImage(img.c_str());
        for (int y = 0; y < rows; y++)
        {
            for (int x = 0; x < cols; x++)
            {
                tileClips.push_back(Rect{x * tWidth, y * tHeight, tWidth, tHeight});
            }
        }
    }
    Tileset::~Tileset()
    {
        for (auto it = collitions.begin(); it != collitions.end();)
        {
            delete it->second;
            it = collitions.erase(it);
        }
    }

    int Tileset::getArea() const
    {
        return tileSize.width * tileSize.height;
    }

    int Tileset::getLastId() const
    {
        return columns * rows - 1;
    }

    Size Tileset::getSize() const
    {
        return tileSize;
    }

    Rect *Tileset::getCollitionBox(int key) const
    {
        if (collitions.find(key) == collitions.end())
            return NULL;
        else
            return collitions.at(key);
    }

    video::RenderTile Tileset::getRenderTile(int x, int y, int t)
    {
        return video::RenderTile{
            .texture = texture,
            .index = index,
            .x = x,
            .y = y,
            .offset_x = tileClips[t].x,
            .offset_y = tileClips[t].y,
            .width = tileClips[t].width,
            .height = tileClips[t].height};
    }

}