#include "intelligence.hpp"

namespace muyuy::ecs::systems
{
    void Intelligence::walkers(entt::registry &reg, map::Map *map)
    {
        const auto view = reg.view<components::Position, components::Movement, components::Rotation, components::Walker, components::Sprite>();

        for (const entt::entity walker : view)
        {
            auto isCharacter = reg.try_get<components::Character>(walker);

            if (!view.get<components::Walker>(walker).isMoving)
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> randT(5, 15);
                view.get<components::Walker>(walker).idleFrames++;
                if (view.get<components::Walker>(walker).idleFrames > randT(gen) * 100)
                {

                    std::uniform_int_distribution<> randX(0, map->getGridSize().width - 1);
                    std::uniform_int_distribution<> randY(0, map->getGridSize().height - 1);
                    GridLocation start = map->getGridPosition(view.get<components::Position>(walker).x, view.get<components::Position>(walker).y);
                    GridLocation goal = GridLocation(randX(gen), randY(gen));
                    auto came_from = PathFinder::search(map->getGrid(), start, goal);
                    auto path = PathFinder::findPath(start, goal, came_from);
                    view.get<components::Walker>(walker).path = path;
                    view.get<components::Walker>(walker).isMoving = true;
                    view.get<components::Walker>(walker).idleFrames = 0;
                }
            }
            else
            {
                GridLocation where = map->getGridPosition(view.get<components::Position>(walker).x, view.get<components::Position>(walker).y);
                std::size_t size = view.get<components::Walker>(walker).path.size();
                GridLocation destiny = view.get<components::Walker>(walker).path.front();
                if (where.x != destiny.x)
                {
                    if (where.x > destiny.x)
                    {
                        view.get<components::Rotation>(walker).direction = 'w';
                        view.get<components::Movement>(walker).southward = false;
                        view.get<components::Movement>(walker).northward = false;
                        view.get<components::Movement>(walker).westward = true;
                        view.get<components::Movement>(walker).eastward = false;
                    }
                    else
                    {
                        view.get<components::Rotation>(walker).direction = 'e';
                        view.get<components::Movement>(walker).southward = false;
                        view.get<components::Movement>(walker).northward = false;
                        view.get<components::Movement>(walker).westward = false;
                        view.get<components::Movement>(walker).eastward = true;
                    }
                }
                else if (where.y != destiny.y)
                {
                    if (where.y > destiny.y)
                    {
                        view.get<components::Rotation>(walker).direction = 'n';
                        view.get<components::Movement>(walker).southward = false;
                        view.get<components::Movement>(walker).northward = true;
                        view.get<components::Movement>(walker).westward = false;
                        view.get<components::Movement>(walker).eastward = false;
                    }
                    else
                    {
                        view.get<components::Rotation>(walker).direction = 's';
                        view.get<components::Movement>(walker).southward = true;
                        view.get<components::Movement>(walker).northward = false;
                        view.get<components::Movement>(walker).westward = false;
                        view.get<components::Movement>(walker).eastward = false;
                    }
                }
                else
                {
                    view.get<components::Walker>(walker).path.erase(view.get<components::Walker>(walker).path.begin());
                    if (view.get<components::Walker>(walker).path.empty())
                    {
                        view.get<components::Movement>(walker).southward = false;
                        view.get<components::Movement>(walker).northward = false;
                        view.get<components::Movement>(walker).westward = false;
                        view.get<components::Movement>(walker).eastward = false;
                        view.get<components::Walker>(walker).isMoving = false;
                    }
                }
            }
        }
    }
}