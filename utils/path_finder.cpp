#include "path_finder.hpp"

namespace PathFinder
{
    std::unordered_map<GridLocation, GridLocation> search(SquareGrid graph, GridLocation start, GridLocation goal)
    {
        std::queue<GridLocation> frontier;
        frontier.push(start);

        std::unordered_map<GridLocation, GridLocation> came_from;
        came_from[start] = start;

        while (!frontier.empty())
        {
            GridLocation current = frontier.front();
            frontier.pop();

            if (current == goal)
            {
                break;
            }

            for (GridLocation next : graph.neighbors(current))
            {
                if (came_from.find(next) == came_from.end())
                {
                    frontier.push(next);
                    came_from[next] = current;
                }
            }
        }
        return came_from;
    }
    std::vector<GridLocation> findPath(GridLocation start, GridLocation goal, std::unordered_map<GridLocation, GridLocation> came_from)
    {
        std::vector<GridLocation> path;
        GridLocation current = goal;
        if (came_from.find(goal) == came_from.end())
        {
            return path; // no path can be found
        }
        while (current != start)
        {
            path.push_back(current);
            current = came_from[current];
        }
        path.push_back(start); // optional
        std::reverse(path.begin(), path.end());
        return path;
    }
};