#include "map.hpp"

namespace muyuy::map
{
    Map::Map(std::string n, std::unordered_map<std::string, Tileset *> *tilesets) : _name(n), _tilesets(tilesets)
    {
        sol::state &lua = script::scriptManager->getGlobalState();

        lua.script_file(n);

        sol::table m = lua["map"];

        sol::table sTilesets = lua["map"]["tilesets"];
        sol::table sLayers = lua["map"]["layers"];

        _map_size = {m["tile_cols"], m["tile_rows"]};
        _tile_size = {m["tilewidth"], m["tileheight"]};

        for (int i = 0; i <= sTilesets.size(); i++)
        {
            sol::table ts = sTilesets[i].get<sol::table>();

            _map_tilesets.push_back(MapTileset{
                .first_id = ts["firstId"].get<int>(),
                .tileset = _tilesets->at(ts["name"].get<std::string>())});
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

            _layers.push_back(new Layer(map, _map_tilesets, Coordinate{m["tile_cols"], m["tile_rows"]}));
        }
    }

    Map::~Map()
    {
        for (auto l : _layers)
        {
            delete l;
        }
        _layers.clear();
    }

    std::vector<Layer *> Map::getLayers() const
    {
        return _layers;
    }

    void Map::draw(Rect &camera) const
    {
        std::vector<video::RenderTile> render;
        for (int i = 0; i < _layers.size(); i++)
        {
            std::vector<video::RenderTile> layers_tiles = _layers[i]->getRenderTiles(camera);
            render.insert(render.end(), layers_tiles.begin(), layers_tiles.end());
        }
        video::videoManager->drawTextureArray(render, "tilesets");
    }

    Size Map::getSize() const
    {
        return Size{_map_size.width * _tile_size.width, _map_size.height * _tile_size.height};
    }

    std::string Map::getName() const
    {
        return _name;
    }

    bool Map::checkCollision(Rect player) const
    {
        bool coll = false;

        for (auto layer : _layers)
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

        for (auto layer : _layers)
        {
            if (layer->getTile(location.x + (location.y) * _map_size.width)->getCollitionBox() != NULL)
            {
                coll = true;
                break;
            }
        }
        return coll;
    }

    Size Map::getGridSize() const
    {
        return _map_size;
    }

    // GridLocation Map::getGridPosition(Coordinate c) const
    // {
    //     return GridLocation(c.x / tileSize.width, c.y / tileSize.height);
    // }

    SquareGrid Map::getGrid() const
    {
        return _grid;
    }
};