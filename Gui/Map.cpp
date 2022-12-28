#include "Map.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <array>

#include "rapidjson/document.h"

using namespace rapidjson;

using namespace std;

Map::Map(string n)
{
    name = n;
    tileset = new TileSet("terrain");
    ifstream map("Resources/maps/" + name + ".tmj");

    if (map.fail())
    {
        printf("Unable to load map file!\n");
    }
    else
    {
        ostringstream sstream;
        sstream << map.rdbuf();
        const string str(sstream.str());
        const char *data = str.c_str();
        Document document;
        document.Parse(data);

        mapSize = {document["width"].GetInt(), document["height"].GetInt()};
        tileSize = {document["tilewidth"].GetInt(), document["tileheight"].GetInt()};
        for (Value::ConstValueIterator itr = document["layers"].Begin(); itr != document["layers"].End(); ++itr)
        {
            const Value &layersDoc = *itr;
            for (Value::ConstValueIterator itr2 = layersDoc["layers"].Begin(); itr2 != layersDoc["layers"].End(); ++itr2)
            {
                const Value &layersDoc2 = *itr2;
                vector<int> layerData;

                for (auto &v : layersDoc2["data"].GetArray())
                    layerData.push_back(v.GetInt());

                if (layersDoc2["visible"].GetBool())
                    layers.push_back(new Layer(layerData, tileset, Coordinate{document["width"].GetInt(), document["height"].GetInt()}));

                // string str = layersDoc2["name"].GetString();
                // if (str == "collisions")
                //     collision = new Collision(layerData, document["tileheight"].GetInt(), document["tilewidth"].GetInt(), document["width"].GetInt(), document["height"].GetInt());
            }
        }
        grid = SquareGrid(document["width"].GetInt(), document["height"].GetInt());
        for (int i = 0; i < document["width"].GetInt(); i++)
            for (int j = 0; j < document["height"].GetInt(); j++)
                if (getCollidersGrid(GridLocation{i, j}))
                    grid.addWall(GridLocation{i, j});
    }
}

Map::~Map()
{
    for (auto l : layers)
    {
        delete l;
    }
    layers.clear();
    delete tileset;
}

vector<Layer *> Map::getLayers() const
{
    return layers;
}

void Map::render(SDL_Rect &camera) const
{

    for (int i = 0; i < layers.size(); i++)
    {
        layers[i]->render(camera);
    }
}

Size Map::getSize() const
{
    return Size{mapSize.w * tileSize.w, mapSize.h * tileSize.h};
}

std::string Map::getName() const
{
    return name;
}

bool Map::getColliders(Coordinate c) const
{
    bool coll = false;

    for (auto layer : layers)
    {
        if (layer->getTile(c.x / tileset->getSize().w + (c.y / tileset->getSize().h) * mapSize.w)->getCollitionBox() != NULL)
        {
            coll = true;
            break;
        }
    }
    return coll;
}

bool Map::getCollidersGrid(GridLocation location) const
{
    bool coll = false;

    for (auto layer : layers)
    {
        if (layer->getTile(location.x + (location.y) * mapSize.w)->getCollitionBox() != NULL)
        {
            coll = true;
            break;
        }
    }
    return coll;
}

Size Map::getGridSize() const
{
    return mapSize;
}

GridLocation Map::getGridPosition(Coordinate c) const
{
    return GridLocation(c.x / tileSize.w, c.y / tileSize.h);
}

SquareGrid Map::getGrid() const
{
    return grid;
}