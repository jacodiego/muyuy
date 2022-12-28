#pragma once

#include "Coordinate.hpp"
#include <cstdint>

enum class Direction : std::uint8_t
{
    up,
    right,
    down,
    left,
    none,
    begin = up,
    end = none
};

// Coordinate toPos(const Direction dir, const int frame = 1)
// {
//     switch (dir)
//     {
//     case Direction::up:
//         return {0, -1 * frame};
//     case Direction::right:
//         return {1 * frame, 0};
//     case Direction::down:
//         return {0, 1 * frame};
//     case Direction::left:
//         return {-1 * frame, 0};
//     default:
//         return {0, 0};
//     }
// }