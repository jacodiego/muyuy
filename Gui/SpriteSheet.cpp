#include "SpriteSheet.hpp"
#include <string>
#include <iostream>
using namespace std;

SpriteSheet::SpriteSheet(string name, Size size, int count, int cols) : name(name), spriteSize(size)
{

    texture.loadFromFile("Resources/images/sprites/" + name + ".png");
    int y = 0;
    int mod_x;

    for (int i = 0; i < count; i++)
    {
        mod_x = i % cols;
        spriteClips.push_back(SDL_Rect{mod_x * size.w, y * size.h, size.w, size.h});

        if (mod_x == cols - 1)
        {
            y++;
        }
    }
}
Size SpriteSheet::getSize() const
{
    return spriteSize;
}

void SpriteSheet::renderSpriteClip(Coordinate c, int t, SDL_Rect &camera)
{
    texture.render(c.x - camera.x, c.y - camera.y, &spriteClips[t]);
}