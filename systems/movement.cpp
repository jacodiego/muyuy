#include "movement.hpp"

namespace muyuy::ecs::systems
{

    void Movement::character(entt::registry &reg, entt::registry &map_reg, Rect &camera, map::Map *map)
    {
        const auto view = reg.view<components::Character, components::Movement, components::Position, components::Sprite, components::Rotation>();
        const auto collisionables = map_reg.view<components::Collisionable, components::Sprite, components::Position>();
        for (const entt::entity e : view)
        {
            int moveX = 0;
            int moveY = 0;
            if (view.get<components::Movement>(e).state != "idle")
            {
                int vel = view.get<components::Movement>(e).state == "running" ? view.get<components::Movement>(e).velocity * 2 : view.get<components::Movement>(e).velocity;
                if (view.get<components::Movement>(e).northward)
                {
                    moveY = vel * -1;
                }
                if (view.get<components::Movement>(e).eastward)
                {
                    moveX = vel;
                }
                if (view.get<components::Movement>(e).southward)
                {
                    moveY = vel;
                }
                if (view.get<components::Movement>(e).westward)
                {
                    moveX = vel * -1;
                }

                view.get<components::Position>(e).x += moveX;
                view.get<components::Position>(e).y += moveY;

                if (view.get<components::Position>(e).x < 0 ||
                    view.get<components::Position>(e).x + view.get<components::Sprite>(e).width > map->getSize().width)
                    view.get<components::Position>(e).x -= moveX;
                if (view.get<components::Position>(e).y < 0 ||
                    view.get<components::Position>(e).y + view.get<components::Sprite>(e).height > map->getSize().height)
                    view.get<components::Position>(e).y -= moveY;

                if (map->checkCollision(Rect{view.get<components::Position>(e).x, view.get<components::Position>(e).y, view.get<components::Sprite>(e).width, view.get<components::Sprite>(e).height}))
                {
                    view.get<components::Position>(e).x -= moveX;
                    view.get<components::Position>(e).y -= moveY;
                }

                for (const entt::entity coll : collisionables)
                {
                    /* if (utils::checkCollision(Rect{view.get<components::Position>(e).x, view.get<components::Position>(e).y, view.get<components::Sprite>(e).width, view.get<components::Sprite>(e).height},
                                            Rect{collisionables.get<components::Position>(coll).x, collisionables.get<components::Position>(coll).y, collisionables.get<components::Sprite>(coll).width, collisionables.get<components::Sprite>(coll).height}))
                    {
                        auto character_position = view.get<components::Position>(e);
                        auto character_sprite = view.get<components::Sprite>(e);
                        auto position = collisionables.get<components::Position>(coll);
                        auto sprite = collisionables.get<components::Sprite>(coll);
                        view.get<components::Position>(e).x -= moveX;
                        view.get<components::Position>(e).y -= moveX;
                        break;
                    } */
                }

                camera.x = (view.get<components::Position>(e).x + view.get<components::Sprite>(e).width / 2) - video::videoManager->getWindowWidth() / 2;
                camera.y = (view.get<components::Position>(e).y + view.get<components::Sprite>(e).height / 2) - video::videoManager->getWindowHeight() / 2;

                if (camera.x < 0)
                {
                    camera.x = 0;
                }
                if (camera.y < 0)
                {
                    camera.y = 0;
                }
                if (camera.x > map->getSize().width - camera.width)
                {
                    camera.x = map->getSize().width - camera.width;
                }
                if (camera.y > map->getSize().height - camera.height)
                {
                    camera.y = map->getSize().height - camera.height;
                }
            }
        }
    }

}