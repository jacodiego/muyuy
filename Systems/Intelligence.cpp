#include "Intelligence.hpp"

void Intelligence::walkers(entt::registry &reg, Map *map)
{
    const auto view = reg.view<Position, Movable, Walker, Sprite>();

    for (const entt::entity walker : view)
    {
        auto isCharacter = reg.try_get<Character>(walker);
        if ((isCharacter != NULL && !isCharacter->dialoguing) || isCharacter == NULL)
        {
            if (!view.get<Walker>(walker).isMoving)
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> randT(5, 15);
                view.get<Walker>(walker).idleFrames++;
                if (view.get<Walker>(walker).idleFrames > randT(gen) * 100)
                {

                    std::uniform_int_distribution<> randX(0, map->getGridSize().w - 1);
                    std::uniform_int_distribution<> randY(0, map->getGridSize().h - 1);
                    GridLocation start = map->getGridPosition(view.get<Position>(walker).c);
                    GridLocation goal = GridLocation(randX(gen), randY(gen));
                    auto came_from = PathFinder::search(map->getGrid(), start, goal);
                    view.get<Walker>(walker).path = PathFinder::findPath(start, goal, came_from);
                    view.get<Walker>(walker).isMoving = true;
                    view.get<Walker>(walker).idleFrames = 0;
                }
            }
            else
            {
                GridLocation where = map->getGridPosition(view.get<Position>(walker).c);
                std::size_t size = view.get<Walker>(walker).path.size();
                GridLocation destiny = view.get<Walker>(walker).path.front();
                if (where.x != destiny.x)
                {
                    if (where.x > destiny.x)
                    {
                        view.get<Movable>(walker).direction = Direction::left;
                        view.get<Movable>(walker).movingDown = false;
                        view.get<Movable>(walker).movingUp = false;
                        view.get<Movable>(walker).movingLeft = true;
                        view.get<Movable>(walker).movingRight = false;
                    }
                    else
                    {
                        view.get<Movable>(walker).direction = Direction::right;
                        view.get<Movable>(walker).movingDown = false;
                        view.get<Movable>(walker).movingUp = false;
                        view.get<Movable>(walker).movingLeft = false;
                        view.get<Movable>(walker).movingRight = true;
                    }
                }
                else if (where.y != destiny.y)
                {
                    if (where.y > destiny.y)
                    {
                        view.get<Movable>(walker).direction = Direction::up;
                        view.get<Movable>(walker).movingDown = false;
                        view.get<Movable>(walker).movingUp = true;
                        view.get<Movable>(walker).movingLeft = false;
                        view.get<Movable>(walker).movingRight = false;
                    }
                    else
                    {
                        view.get<Movable>(walker).direction = Direction::down;
                        view.get<Movable>(walker).movingDown = true;
                        view.get<Movable>(walker).movingUp = false;
                        view.get<Movable>(walker).movingLeft = false;
                        view.get<Movable>(walker).movingRight = false;
                    }
                }
                else
                {
                    view.get<Walker>(walker).path.erase(view.get<Walker>(walker).path.begin());
                    if (view.get<Walker>(walker).path.empty())
                    {
                        view.get<Movable>(walker).movingDown = false;
                        view.get<Movable>(walker).movingUp = false;
                        view.get<Movable>(walker).movingLeft = false;
                        view.get<Movable>(walker).movingRight = false;
                        view.get<Walker>(walker).isMoving = false;
                    }
                }
            }
        }
    }
}
