#pragma once

#include "utils/grid.hpp"

namespace muyuy::ecs::components
{
    struct Walker
    {
        bool isMoving = false;
        int idleFrames = 0;
        std::vector<GridLocation> path;
    };
}