#include "grid.hpp"

SquareGrid::SquareGrid()
    : width(0), height(0)
{
}

SquareGrid::SquareGrid(int width_, int height_)
    : width(width_), height(height_)
{
}

void SquareGrid::addWall(GridLocation location)
{
    walls.insert(location);
}

bool SquareGrid::in_bounds(GridLocation id) const
{
    return 0 <= id.x && id.x < width && 0 <= id.y && id.y < height;
}

bool SquareGrid::passable(GridLocation id) const
{
    return walls.find(id) == walls.end();
}

std::vector<GridLocation> SquareGrid::neighbors(GridLocation id) const
{
    std::vector<GridLocation> results;

    for (GridLocation dir : DIRS)
    {
        GridLocation next{id.x + dir.x, id.y + dir.y};
        if (in_bounds(next) && passable(next))
        {
            results.push_back(next);
        }
    }

    if ((id.x + id.y) % 2 == 0)
    {
        // see "Ugly paths" section for an explanation:
        std::reverse(results.begin(), results.end());
    }

    return results;
}