#include "Render.hpp"

#include "../Components/Sprite.hpp"
#include "../Components/Position.hpp"
#include "../Components/Movable.hpp"
#include "../Components/Item.hpp"
#include "../Components/Openable.hpp"
#include <entt/entity/registry.hpp>
#include "iostream"
#include "iomanip"

void Render::movable(entt::registry &reg, const int frame, SDL_Rect &camera)
{
    const auto view = reg.view<Position, Movable, Sprite>();
    for (const entt::entity e : view)
    {
        int sprite = 7;
        if (view.get<Movable>(e).direction == Direction::up)
            sprite = 1;
        if (view.get<Movable>(e).direction == Direction::right)
            sprite = 4;
        if (view.get<Movable>(e).direction == Direction::down)
            sprite = 7;
        if (view.get<Movable>(e).direction == Direction::left)
            sprite = 10;

        if (view.get<Movable>(e).direction == Direction::up && view.get<Movable>(e).movingUp)
            sprite += (frame % 3) - 1;
        if (view.get<Movable>(e).direction == Direction::right && view.get<Movable>(e).movingRight)
            sprite += (frame % 3) - 1;
        if (view.get<Movable>(e).direction == Direction::down && view.get<Movable>(e).movingDown)
            sprite += (frame % 3) - 1;
        if (view.get<Movable>(e).direction == Direction::left && view.get<Movable>(e).movingLeft)
            sprite += (frame % 3) - 1;

        view.get<Sprite>(e).spriteSheet->renderSpriteClip(view.get<Position>(e).c, sprite, camera);
    }
}

void Render::treasures(entt::registry &reg, SDL_Rect &camera)
{
    const auto view = reg.view<Item, Position, Sprite, Openable>();
    for (const entt::entity e : view)
    {
        int sprite = 0;
        if (view.get<Openable>(e).isOpen)
            sprite = 1;

        view.get<Sprite>(e).spriteSheet->renderSpriteClip(view.get<Position>(e).c, sprite, camera);
    }
}
