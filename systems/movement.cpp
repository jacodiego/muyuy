#include "movement.hpp"

namespace muyuy::ecs::systems
{

    void Movement::character(entt::registry &reg, entt::registry &map_reg, Rect &camera, map::Map *map)
    {
        const auto view = reg.view<components::Character, components::Movement, components::Position, components::Sprite, components::Rotation>();
        const auto collisionables = map_reg.view<components::Collisionable, components::Sprite, components::Position>();
        for (const entt::entity e : view)
        {
            auto &character = view.get<components::Character>(e);
            if (character.active)
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
                        if (utils::checkCollision(Rect{view.get<components::Position>(e).x, view.get<components::Position>(e).y, view.get<components::Sprite>(e).width, view.get<components::Sprite>(e).height},
                                                  Rect{collisionables.get<components::Position>(coll).x, collisionables.get<components::Position>(coll).y, collisionables.get<components::Sprite>(coll).width, collisionables.get<components::Sprite>(coll).height}))
                        {
                            /* auto character_position = view.get<components::Position>(e);
                            auto character_sprite = view.get<components::Sprite>(e);
                            auto position = collisionables.get<components::Position>(coll);
                            auto sprite = collisionables.get<components::Sprite>(coll); */
                            view.get<components::Position>(e).x -= moveX;
                            view.get<components::Position>(e).y -= moveY;
                            break;
                        }
                    }
                }
            }
        }
    }

    void Movement::walkers(entt::registry &reg, map::Map *map)
    {
        const auto view = reg.view<components::Position, components::Movement, components::Walker, components::Sprite>();
        const auto collisionables = reg.view<components::Collisionable, components::Sprite, components::Position>();

        for (const entt::entity walker : view)
        {
            // auto isCharacter = reg.try_get<components::Character>(walker);
            /* if ((isCharacter != NULL && !isCharacter->dialoguing) || isCharacter == NULL)
            {
                int velX = 0;
                int velY = 0;
                if (view.get<Movable>(walker).movingUp)
                    velY = -1;
                if (view.get<Movable>(walker).movingDown)
                    velY = 1;
                if (view.get<Movable>(walker).movingLeft)
                    velX = -1;
                if (view.get<Movable>(walker).movingRight)
                    velX = 1;

                view.get<Position>(walker).c.y += velY;
                view.get<Position>(walker).c.x += velX;
            } */

            int velX = 0;
            int velY = 0;
            if (view.get<components::Movement>(walker).northward)
                velY = -1;
            if (view.get<components::Movement>(walker).southward)
                velY = 1;
            if (view.get<components::Movement>(walker).westward)
                velX = -1;
            if (view.get<components::Movement>(walker).eastward)
                velX = 1;

            view.get<components::Position>(walker).y += velY;
            view.get<components::Position>(walker).x += velX;
            // if (view.get<Position>(character).c.x < 0 ||
            //     view.get<Position>(character).c.x + view.get<Sprite>(character).spriteSheet->getSize().w > map->getSize().w ||
            //     map->getColliders(view.get<Position>(character).c))
            //     view.get<Position>(character).c.x -= velX;
            // if (view.get<Position>(character).c.y < 0 ||
            //     view.get<Position>(character).c.y + view.get<Sprite>(character).spriteSheet->getSize().h > map->getSize().h ||
            //     map->getColliders(view.get<Position>(character).c))
            //     view.get<Position>(character).c.y -= velY;

            // for (const entt::entity coll : collisionables)
            // {
            //     if (Util::checkCollision(SDL_Rect{view.get<Position>(character).c.x, view.get<Position>(character).c.y, view.get<Sprite>(character).spriteSheet->getSize().w, view.get<Sprite>(character).spriteSheet->getSize().h},
            //                              SDL_Rect{collisionables.get<Position>(coll).c.x, collisionables.get<Position>(coll).c.y, collisionables.get<Sprite>(coll).spriteSheet->getSize().w, collisionables.get<Sprite>(coll).spriteSheet->getSize().h}))
            //     {
            //         view.get<Position>(character).c.x -= velX;
            //         view.get<Position>(character).c.y -= velY;
            //         break;
            //     }
            // }
        }
    }

}