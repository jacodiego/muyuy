#pragma once

struct Coordinate
{
    int x, y;

    Coordinate(int x = 0, int y = 0) : x(x), y(y)
    {
    }

    bool operator==(const Coordinate &coordinate) const
    {
        return (coordinate.x == x && coordinate.y == y);
    }

    Coordinate operator+(const Coordinate &coordinate) const
    {
        return Coordinate(coordinate.x + x, coordinate.y + y);
    }
};