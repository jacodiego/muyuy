#include "layer.hpp"

namespace muyuy::map
{
    Layer::Layer(std::vector<std::vector<int>> &tTypes, std::vector<MapTileset> &ts, Coordinate lSize)
        : tilesets(ts), layerSize(lSize)
    {
        for (int y = 0; y < tTypes.size(); y++)
        {
            std::vector<Tile *> rowMap;
            for (int x = 0; x < tTypes[y].size(); x++)
            {
                int tileType = tTypes[y][x];
                if (tileType != -1)
                {
                    Tileset *usedTs = NULL;
                    int firstId = 0;
                    for (auto tSet : ts)
                    {
                        if (tileType >= tSet.first_id && tileType <= tSet.first_id + tSet.tileset->getLastId())
                        {
                            usedTs = tSet.tileset;
                            firstId = tSet.first_id;
                            break;
                        }
                    }
                    tileMap.push_back(new Tile(Coordinate{x * usedTs->getSize().width, y * usedTs->getSize().height}, usedTs, tileType - firstId));
                }
                else
                {
                    tileMap.push_back(NULL);
                }
            }
        }
    }
    Layer::~Layer()
    {
        for (auto tm : tileMap)
        {
            delete tm;
        }
        tileMap.clear();
    }

    std::vector<video::RenderTile> Layer::getRenderTiles(Rect &camera) const
    {
        std::vector<video::RenderTile> render;
        for (int i = 0; i < tileMap.size(); i++)
        {
            if (tileMap[i] != NULL)
            {
                video::RenderTile renderTile = tileMap[i]->getRenderTile(camera);
                render.push_back(renderTile);
            }
        }
        return render;
    }

    Tile *Layer::getTile(int i) const
    {
        return tileMap[i];
    }

    std::vector<Rect> Layer::getColliders() const
    {
        std::vector<Rect> colls;
        for (auto tm : tileMap)
        {
            if (tm != NULL && tm->getCollitionBox() != NULL)
                colls.push_back(Rect{tm->getBox().x + tm->getCollitionBox()->x, tm->getBox().y + tm->getCollitionBox()->y, tm->getCollitionBox()->width, tm->getCollitionBox()->height});
        }
        return colls;
    }
}