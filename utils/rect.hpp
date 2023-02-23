#pragma once

struct Rect
{
    int x, y, width, height;
    Rect()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }

    Rect(int x_, int y_, int width_, int height_)
    {
        x = x_;
        y = y_;
        width = width_;
        height = height_;
    }
};