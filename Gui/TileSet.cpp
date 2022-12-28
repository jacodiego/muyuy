#include "TileSet.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

TileSet::TileSet(string name) : name(name), texture(Texture{})
{
    ifstream tileSetJson("Resources/tilesets/" + name + ".tsj");
    if (tileSetJson.fail())
    {
        printf("Unable to load map file!\n");
    }
    else
    {
        ostringstream sstream;
        sstream << tileSetJson.rdbuf();
        const string str(sstream.str());
        const char *data = str.c_str();
        Document document;
        document.Parse(data);
        texture.loadFromFile(document["image"].GetString());
        tileSize.w = document["tilewidth"].GetInt();
        tileSize.h = document["tileheight"].GetInt();
        int y = 0;
        int mod_x;
        for (int i = 0; i < document["tilecount"].GetInt(); i++)
        {
            mod_x = i % document["columns"].GetInt();
            tileClips.push_back(SDL_Rect{mod_x * tileSize.w, y * tileSize.h, tileSize.w, tileSize.h});

            if (mod_x == document["columns"].GetInt() - 1)
            {
                y++;
            }
        }

        if (document.HasMember("tiles"))
        {
            const Value &objs = document["tiles"];
            if (objs.IsArray())
            {
                for (SizeType j = 0; j < objs.Size(); j++)
                {
                    if (objs[j].IsObject())
                    {
                        const Value &coll = objs[j]["objectgroup"]["objects"][0];
                        collitions.insert(std::make_pair(objs[j]["id"].GetInt(), new SDL_Rect((int)coll["x"].GetFloat(),
                                                                                              (int)coll["y"].GetFloat(),
                                                                                              (int)coll["width"].GetFloat(),
                                                                                              (int)coll["height"].GetFloat())));
                    }
                }
            }
        }
    }
}
TileSet::~TileSet()
{
    for (auto it = collitions.begin(); it != collitions.end();)
    {
        delete it->second;
        it = collitions.erase(it);
    }
}

int TileSet::getArea() const
{
    return tileSize.h * tileSize.w;
}

Size TileSet::getSize() const
{
    return tileSize;
}

SDL_Rect *TileSet::getCollitionBox(int key) const
{
    if (collitions.find(key) == collitions.end())
        return NULL;
    else
        return collitions.at(key);
}

void TileSet::renderTileClip(int x, int y, int t)
{
    if (t != -1)
        texture.render(x, y, &tileClips[t]);
}