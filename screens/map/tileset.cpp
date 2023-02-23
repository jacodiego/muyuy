#include "tileset.hpp"

namespace muyuy::map
{
    Tileset::Tileset(std::string name, std::string img, int cols, int rows, int tWidth, int tHeight, int fId, std::map<int, Rect *> colls)
        : name(name), columns(cols), rows(rows), tileSize({tWidth, tHeight}), firstId(fId), collitions(colls)
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
        return firstId + columns * rows - 1;
    }

    int Tileset::getFirstId() const
    {
        return firstId;
    }

    Size Tileset::getSize() const
    {
        return tileSize;
    }

    Rect *Tileset::getCollitionBox(int key) const
    {
        if (collitions.find(key - firstId) == collitions.end())
            return NULL;
        else
            return collitions.at(key - firstId);
    }

    video::RenderTile Tileset::getRenderTile(int x, int y, int t)
    {
        // texture->draw(x, y, tileClips[t - firstId].x, tileClips[t - firstId].y, tileClips[t - firstId].width, tileClips[t - firstId].height, 1.0f, 1.0f, 0.0f);
        return video::RenderTile{
            .texture = texture,
            .x = x,
            .y = y,
            .offset_x = tileClips[t - firstId].x,
            .offset_y = tileClips[t - firstId].y,
            .width = tileClips[t - firstId].width,
            .height = tileClips[t - firstId].height};
    }

}