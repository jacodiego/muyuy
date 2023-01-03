#include "TileSet.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

TileSet::TileSet(string name, string img, int cols, int rows, int tWidth, int tHeight, int fId, std::map<int, SDL_Rect *> colls)
    : name(name), columns(cols), rows(rows), tileSize({tWidth, tHeight}), firstId(fId), texture(Texture{}), collitions(colls)
{
    texture.loadFromFile(img);
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            tileClips.push_back(SDL_Rect{x * tWidth, y * tHeight, tWidth, tHeight});
        }
    }
}
// TileSet::TileSet(string name) : name(name), texture(Texture{})
// {
//     ifstream tileSetJson("Resources/tilesets/" + name + ".tsj");
//     if (tileSetJson.fail())
//     {
//         printf("Unable to load map file!\n");
//     }
//     else
//     {
//         ostringstream sstream;
//         sstream << tileSetJson.rdbuf();
//         const string str(sstream.str());
//         const char *data = str.c_str();
//         Document document;
//         document.Parse(data);
//         texture.loadFromFile(document["image"].GetString());
//         tileSize.w = document["tilewidth"].GetInt();
//         tileSize.h = document["tileheight"].GetInt();
//         int y = 0;
//         int mod_x;
//         for (int i = 0; i < document["tilecount"].GetInt(); i++)
//         {
//             mod_x = i % document["columns"].GetInt();
//             tileClips.push_back(SDL_Rect{mod_x * tileSize.w, y * tileSize.h, tileSize.w, tileSize.h});

//             if (mod_x == document["columns"].GetInt() - 1)
//             {
//                 y++;
//             }
//         }

//         if (document.HasMember("tiles"))
//         {
//             const Value &objs = document["tiles"];
//             if (objs.IsArray())
//             {
//                 for (SizeType j = 0; j < objs.Size(); j++)
//                 {
//                     if (objs[j].IsObject())
//                     {
//                         const Value &coll = objs[j]["objectgroup"]["objects"][0];
//                         collitions.insert(std::make_pair(objs[j]["id"].GetInt(), new SDL_Rect((int)coll["x"].GetFloat(),
//                                                                                               (int)coll["y"].GetFloat(),
//                                                                                               (int)coll["width"].GetFloat(),
//                                                                                               (int)coll["height"].GetFloat())));
//                     }
//                 }
//             }
//         }
//     }
// }
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

int TileSet::getLastId() const
{
    return firstId + columns * rows - 1;
}

int TileSet::getFirstId() const
{
    return firstId;
}

Size TileSet::getSize() const
{
    return tileSize;
}

SDL_Rect *TileSet::getCollitionBox(int key) const
{
    if (collitions.find(key - firstId) == collitions.end())
        return NULL;
    else
        return collitions.at(key - firstId);
}

void TileSet::renderTileClip(int x, int y, int t)
{
    texture.render(x, y, &tileClips[t - firstId]);
}