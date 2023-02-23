#include "map.hpp"

namespace muyuy::map
{
    Map::Map(std::string n) : name(n)
    {
        sol::state lua;
        lua.open_libraries(sol::lib::base);

        lua.script_file(n);

        sol::table m = lua["map"];

        sol::table sTilesets = lua["map"]["tilesets"];
        sol::table sLayers = lua["map"]["layers"];

        mapSize = {m["tile_cols"], m["tile_rows"]};
        tileSize = {m["tilewidth"], m["tileheight"]};

        for (int i = 0; i <= sTilesets.size(); i++)
        {
            sol::table ts = sTilesets[i].get<sol::table>();

            lua.script_file(ts["path"].get<std::string>());

            sol::table tileset = lua["tileset"];

            std::map<int, Rect *> collitions;
            sol::table colls = tileset["colliders"].get<sol::table>();

            for (int c = 0; c <= colls.size(); c++)
            {
                collitions.insert(std::make_pair(colls[c]["id"].get<int>(), new Rect(colls[c]["x"].get<int>(),
                                                                                     colls[c]["y"].get<int>(),
                                                                                     colls[c]["width"].get<int>(),
                                                                                     colls[c]["height"].get<int>())));
            }

            tilesets.push_back(new Tileset(
                tileset["name"].get<std::string>(),
                tileset["image"].get<std::string>(),
                tileset["columns"].get<int>(),
                tileset["rows"].get<int>(),
                tileset["tilewidth"].get<int>(),
                tileset["tileheight"].get<int>(),
                ts["firstId"].get<int>(),
                collitions));
        }

        for (int l = 0; l <= sLayers.size(); l++)
        {
            std::vector<std::vector<int>> map;
            sol::table lay = sLayers[l].get<sol::table>();

            for (int t = 0; t <= lay.size(); t++)
            {
                std::vector<int> row = lay[t].get<std::vector<int>>();
                map.push_back(row);
            }

            layers.push_back(new Layer(map, tilesets, Coordinate{m["tile_cols"], m["tile_rows"]}));
        }
    }

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

    std::vector<Layer *> Map::getLayers() const
    {
        return layers;
    }

    void Map::draw(Rect &camera) const
    {
        for (int i = 0; i < layers.size(); i++)
        {
            layers[i]->draw(camera);
        }
    }

    Size Map::getSize() const
    {
        return Size{mapSize.width * tileSize.width, mapSize.height * tileSize.height};
    }

    std::string Map::getName() const
    {
        return name;
    }

    bool Map::checkCollision(Rect player) const
    {
        bool coll = false;

        for (auto layer : layers)
        {
            for (Rect collider : layer->getColliders())
            {
                // if (Util::checkCollision(player, collider))
                // {
                //     coll = true;
                //     break;
                // }
            }
        }
        return coll;
    }

    bool Map::getCollidersGrid(GridLocation location) const
    {
        bool coll = false;

        for (auto layer : layers)
        {
            if (layer->getTile(location.x + (location.y) * mapSize.width)->getCollitionBox() != NULL)
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

    // GridLocation Map::getGridPosition(Coordinate c) const
    // {
    //     return GridLocation(c.x / tileSize.width, c.y / tileSize.height);
    // }

    SquareGrid Map::getGrid() const
    {
        return grid;
    }
};