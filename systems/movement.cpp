#include "movement.hpp"

namespace muyuy::ecs::systems
{

    void Movement::character(entt::registry &reg, Rect &camera)
    {
        const auto view = reg.view<components::Character, components::Move, components::Position, components::State>();
        for (const entt::entity e : view)
        {
            view.get<components::State>(e).name = "idle";
            if (view.get<components::Move>(e).northward)
            {
                view.get<components::State>(e) = {.name = view.get<components::Move>(e).running ? "running" : "walking",
                                                  .description = "north"};
                view.get<components::Position>(e).y -= view.get<components::Move>(e).running ? view.get<components::Move>(e).velocity * 2 : view.get<components::Move>(e).velocity;
            }
            if (view.get<components::Move>(e).eastward)
            {
                view.get<components::State>(e) = {.name = view.get<components::Move>(e).running ? "running" : "walking",
                                                  .description = "east"};
                view.get<components::Position>(e).x += view.get<components::Move>(e).running ? view.get<components::Move>(e).velocity * 2 : view.get<components::Move>(e).velocity;
            }
            if (view.get<components::Move>(e).southward)
            {
                view.get<components::State>(e) = {.name = view.get<components::Move>(e).running ? "running" : "walking",
                                                  .description = "south"};
                view.get<components::Position>(e).y += view.get<components::Move>(e).running ? view.get<components::Move>(e).velocity * 2 : view.get<components::Move>(e).velocity;
            }
            if (view.get<components::Move>(e).westward)
            {
                view.get<components::State>(e) = {.name = view.get<components::Move>(e).running ? "running" : "walking",
                                                  .description = "west"};
                view.get<components::Position>(e).x -= view.get<components::Move>(e).running ? view.get<components::Move>(e).velocity * 2 : view.get<components::Move>(e).velocity;
            }
        }
    }

}