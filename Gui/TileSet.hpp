#pragma once

#include "../Utilities/Size.hpp"
#include "Texture.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <map>
#include <utility>

class TileSet
{
public:
    TileSet(std::string, std::string, int, int, int, int, int, std::map<int, SDL_Rect *>);
    ~TileSet();
    int getArea() const;
    int getFirstId() const;
    int getLastId() const;
    Size getSize() const;
    SDL_Rect *getCollitionBox(int) const;
    void renderTileClip(int, int, int);

private:
    Texture texture;
    std::string name;
    Size tileSize;
    int columns;
    int rows;
    int firstId;
    std::vector<SDL_Rect> tileClips;
    std::map<int, SDL_Rect *> collitions;
};