#pragma once

#include "../Utilities/Grid.hpp"

struct Walker
{
    bool isMoving = false;
    int idleFrames = 0;
    std::vector<GridLocation> path;
};