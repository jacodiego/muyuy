#include "layer.hpp"

namespace muyuy::map
{
    Layer::Layer(std::vector<std::vector<int>> &tTypes, std::vector<Tileset *> &ts, Coordinate lSize)
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
                    for (auto tSet : ts)
                    {
                        if (tileType >= tSet->getFirstId() && tileType <= tSet->getLastId())
                        {
                            usedTs = tSet;
                            break;
                        }
                    }
                    tileMap.push_back(new Tile(Coordinate{x * usedTs->getSize().width, y * usedTs->getSize().height}, usedTs, tileType));
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

    void Layer::draw(Rect &camera) const
    {
        std::map<video::Texture *, std::vector<video::RenderTile>> render;
        for (int i = 0; i < tileMap.size(); i++)
        {
            if (tileMap[i] != NULL)
            {
                video::RenderTile renderTile = tileMap[i]->getRenderTile(camera);
                render[renderTile.texture].push_back(renderTile);
            }
        }
        video::videoManager->drawTiles(render);
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