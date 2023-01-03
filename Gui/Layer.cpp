#include "Layer.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

using namespace std;

Layer::Layer(vector<vector<int>> &tTypes, vector<TileSet *> &ts, Coordinate lSize)
    : tilesets(ts), layerSize(lSize)
{
    for (int y = 0; y < tTypes.size(); y++)
    {
        vector<Tile *> rowMap;
        for (int x = 0; x < tTypes[y].size(); x++)
        {
            int tileType = tTypes[y][x];
            if (tileType != -1)
            {
                TileSet *usedTs = NULL;
                for (auto tSet : ts)
                {
                    if (tileType >= tSet->getFirstId() && tileType <= tSet->getLastId())
                    {
                        usedTs = tSet;
                        break;
                    }
                }
                tileMap.push_back(new Tile(Coordinate{x * usedTs->getSize().w, y * usedTs->getSize().h}, usedTs, tileType));
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

void Layer::render(SDL_Rect &camera) const
{
    for (int i = 0; i < tileMap.size(); i++)
    {
        if (tileMap[i] != NULL)
            tileMap[i]->render(camera);
    }
}

Tile *Layer::getTile(int i) const
{
    return tileMap[i];
}

std::vector<SDL_Rect> Layer::getColliders() const
{
    std::vector<SDL_Rect> colls;
    for (auto tm : tileMap)
    {
        if (tm != NULL && tm->getCollitionBox() != NULL)
            colls.push_back(SDL_Rect{tm->getBox().x + tm->getCollitionBox()->x, tm->getBox().y + tm->getCollitionBox()->y, tm->getCollitionBox()->w, tm->getCollitionBox()->h});
    }
    return colls;
}