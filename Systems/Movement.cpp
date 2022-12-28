#include "Movement.hpp"

void Movement::hero(entt::registry &reg, SDL_Rect &camera, Map *map)
{
    const auto view = reg.view<Hero, Position, Movable, Sprite>();
    const auto hero = *view.begin();

    const auto collisionables = reg.view<Collisionable, Sprite, Position>();

    int velX = 0;
    int velY = 0;
    if (view.get<Movable>(hero).movingUp)
        velY = -2;
    if (view.get<Movable>(hero).movingDown)
        velY = 2;
    if (view.get<Movable>(hero).movingLeft)
        velX = -2;
    if (view.get<Movable>(hero).movingRight)
        velX = 2;

    view.get<Position>(hero).c.y += velY;
    view.get<Position>(hero).c.x += velX;

    if (view.get<Position>(hero).c.x < 0 ||
        view.get<Position>(hero).c.x + view.get<Sprite>(hero).spriteSheet->getSize().w > map->getSize().w ||
        map->getColliders(view.get<Position>(hero).c))
        view.get<Position>(hero).c.x -= velX;
    if (view.get<Position>(hero).c.y < 0 ||
        view.get<Position>(hero).c.y + view.get<Sprite>(hero).spriteSheet->getSize().h > map->getSize().h ||
        map->getColliders(view.get<Position>(hero).c))
        view.get<Position>(hero).c.y -= velY;

    for (const entt::entity coll : collisionables)
    {
        if (Util::checkCollision(SDL_Rect{view.get<Position>(hero).c.x, view.get<Position>(hero).c.y, view.get<Sprite>(hero).spriteSheet->getSize().w, view.get<Sprite>(hero).spriteSheet->getSize().h},
                                 SDL_Rect{collisionables.get<Position>(coll).c.x, collisionables.get<Position>(coll).c.y, collisionables.get<Sprite>(coll).spriteSheet->getSize().w, collisionables.get<Sprite>(coll).spriteSheet->getSize().h}))
        {
            view.get<Position>(hero).c.x -= velX;
            view.get<Position>(hero).c.y -= velY;
            break;
        }
    }

    camera.x = (view.get<Position>(hero).c.x + view.get<Sprite>(hero).spriteSheet->getSize().w / 2) - Graphics::getScreenSize().w / 2;
    camera.y = (view.get<Position>(hero).c.y + view.get<Sprite>(hero).spriteSheet->getSize().h / 2) - Graphics::getScreenSize().h / 2;

    // Keep the camera in bounds
    if (camera.x < 0)
    {
        camera.x = 0;
    }
    if (camera.y < 0)
    {
        camera.y = 0;
    }
    if (camera.x > map->getSize().w - camera.w)
    {
        camera.x = map->getSize().w - camera.w;
    }
    if (camera.y > map->getSize().h - camera.h)
    {
        camera.y = map->getSize().h - camera.h;
    }
    map->getGridPosition(view.get<Position>(hero).c);
}

void Movement::walkers(entt::registry &reg, Map *map)
{
    const auto view = reg.view<Position, Movable, Walker, Sprite>();
    const auto collisionables = reg.view<Collisionable, Sprite, Position>();

    for (const entt::entity walker : view)
    {
        auto isCharacter = reg.try_get<Character>(walker);
        if ((isCharacter != NULL && !isCharacter->dialoguing) || isCharacter == NULL)
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
        }

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
