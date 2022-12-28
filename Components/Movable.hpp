#pragma once

#include "../Utilities/Direction.hpp"

struct Movable
{
    Direction direction = Direction::none;
    bool movingUp = false;
    bool movingDown = false;
    bool movingLeft = false;
    bool movingRight = false;
};