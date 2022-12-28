#include "Layer.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

using namespace std;

Layer::Layer(vector<int> tTypes, TileSet *ts, Coordinate lSize)
    : tileTypes(tTypes), tileset(ts), layerSize(lSize)
{

    int x = 0, y = 0;
    for (int i = 0; i < tTypes.size(); ++i)
    {
        int tileType = tTypes[i];

        if ((tileType >= 0) && (tileType <= ts->getArea()))
        {
            tileMap.push_back(new Tile(Coordinate{x, y}, ts, tileType - 1));
        }
        else
        {
            printf("Error loading map: Invalid tile type at %d!\n");
            break;
        }
        x += ts->getSize().w;

        if (x >= ts->getSize().w * layerSize.x)
        {
            x = 0;
            y += ts->getSize().h;
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

vector<Tile *> Layer::getTileMap() const
{
    return tileMap;
}

void Layer::render(SDL_Rect &camera) const
{
    for (int i = 0; i < layerSize.x * layerSize.y; i++)
    {
        tileMap[i]->render(camera);
    }
}

Tile *Layer::getTile(int i) const
{
    return tileMap[i];
}