#include "Map.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <array>
#include <sol/sol.hpp>

using namespace std;

Map::Map(string n) : name(n)
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    lua.script_file("Data/Maps/" + n + ".lua");

    sol::table m = lua["map"];

    sol::table sTilesets = lua["map"]["tilesets"];
    sol::table sLayers = lua["map"]["layers"];

    mapSize = {m["tile_cols"], m["tile_rows"]};
    tileSize = {m["tilewidth"], m["tileheight"]};

    for (int i = 0; i <= sTilesets.size(); i++)
    {
        sol::table ts = sTilesets[i].get<sol::table>();

        lua.script_file(ts["path"].get<string>());

        sol::table tileset = lua["tileset"];

        std::map<int, SDL_Rect *> collitions;
        sol::table colls = tileset["colliders"].get<sol::table>();

        for (int c = 0; c <= colls.size(); c++)
        {
            collitions.insert(std::make_pair(colls[c]["id"].get<int>(), new SDL_Rect(colls[c]["x"].get<int>(),
                                                                                     colls[c]["y"].get<int>(),
                                                                                     colls[c]["width"].get<int>(),
                                                                                     colls[c]["height"].get<int>())));
        }

        tilesets.push_back(new TileSet(
            tileset["name"].get<string>(),
            tileset["image"].get<string>(),
            tileset["columns"].get<int>(),
            tileset["rows"].get<int>(),
            tileset["tilewidth"].get<int>(),
            tileset["tileheight"].get<int>(),
            ts["firstId"].get<int>(),
            collitions));
    }

    for (int l = 0; l <= sLayers.size(); l++)
    {
        vector<vector<int>> map;
        sol::table lay = sLayers[l].get<sol::table>();

        for (int t = 0; t <= lay.size(); t++)
        {
            vector<int> row = lay[t].get<vector<int>>();
            map.push_back(row);
        }

        layers.push_back(new Layer(map, tilesets, Coordinate{m["tile_cols"], m["tile_rows"]}));
    }
}

//         grid = SquareGrid(document["width"].GetInt(), document["height"].GetInt());
//         for (int i = 0; i < document["width"].GetInt(); i++)
//             for (int j = 0; j < document["height"].GetInt(); j++)
//                 if (getCollidersGrid(GridLocation{i, j}))
//                     grid.addWall(GridLocation{i, j});

Map::~Map()
{
    for (auto l : layers)
    {
        delete l;
    }
    layers.clear();
    for (auto ts : tilesets)
    {
        delete ts;
    }
    tilesets.clear();
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

bool Map::checkCollision(SDL_Rect player) const
{
    bool coll = false;

    for (auto layer : layers)
    {
        for (SDL_Rect collider : layer->getColliders())
        {
            if (Util::checkCollision(player, collider))
            {
                coll = true;
                break;
            }
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