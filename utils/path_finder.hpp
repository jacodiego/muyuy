#pragma once

#include "utils/grid.hpp"
#include <unordered_map>
#include <vector>
#include <queue>

namespace PathFinder
{
    std::unordered_map<GridLocation, GridLocation> search(SquareGrid, GridLocation, GridLocation);
    std::vector<GridLocation> findPath(GridLocation, GridLocation, std::unordered_map<GridLocation, GridLocation>);
};