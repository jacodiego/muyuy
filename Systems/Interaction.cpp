#include "Interaction.hpp"

void Interaction::open(entt::registry &reg)
{
    const auto view = reg.view<Openable, Position, Sprite>();
    const auto heroView = reg.view<Hero, Position, Sprite, Movable>();
    const auto h = *heroView.begin();
    for (const entt::entity e : view)
    {
        int focusX = 0;
        int focusY = 0;
        switch (heroView.get<Movable>(h).direction)
        {
        case Direction::up:
            focusY = -5;
            break;
        case Direction::down:
            focusY = 5;
            break;
        case Direction::left:
            focusX = -5;
            break;
        case Direction::right:
            focusX = 5;
            break;
        default:
            break;
        }
        if (Util::checkCollision(SDL_Rect{view.get<Position>(e).c.x, view.get<Position>(e).c.y, view.get<Sprite>(e).spriteSheet->getSize().w, view.get<Sprite>(e).spriteSheet->getSize().h},
                                 SDL_Rect{heroView.get<Position>(h).c.x + focusX, heroView.get<Position>(h).c.y + focusY, heroView.get<Sprite>(h).spriteSheet->getSize().w, heroView.get<Sprite>(h).spriteSheet->getSize().h}))
            view.get<Openable>(e).isOpen = !view.get<Openable>(e).isOpen;
    }
}

std::string Interaction::talk(entt::registry &reg)
{
    const auto view = reg.view<Character, Position, Sprite, Movable>();
    const auto heroView = reg.view<Hero, Position, Sprite, Movable>();
    const auto h = *heroView.begin();
    for (const entt::entity e : view)
    {
        int focusX = 0;
        int focusY = 0;
        switch (heroView.get<Movable>(h).direction)
        {
        case Direction::up:
            focusY = -5;
            break;
        case Direction::down:
            focusY = 5;
            break;
        case Direction::left:
            focusX = -5;
            break;
        case Direction::right:
            focusX = 5;
            break;
        default:
            break;
        }
        if (Util::checkCollision(SDL_Rect{view.get<Position>(e).c.x, view.get<Position>(e).c.y, view.get<Sprite>(e).spriteSheet->getSize().w, view.get<Sprite>(e).spriteSheet->getSize().h},
                                 SDL_Rect{heroView.get<Position>(h).c.x + focusX, heroView.get<Position>(h).c.y + focusY, heroView.get<Sprite>(h).spriteSheet->getSize().w, heroView.get<Sprite>(h).spriteSheet->getSize().h}))
        {
            if (heroView.get<Position>(h).c.y + heroView.get<Sprite>(h).spriteSheet->getSize().h <= view.get<Position>(e).c.y)
                view.get<Movable>(e).direction = Direction::up;
            if (heroView.get<Position>(h).c.x >= view.get<Position>(e).c.x + view.get<Sprite>(e).spriteSheet->getSize().w)
                view.get<Movable>(e).direction = Direction::right;
            if (heroView.get<Position>(h).c.y >= view.get<Position>(e).c.y + view.get<Sprite>(e).spriteSheet->getSize().h)
                view.get<Movable>(e).direction = Direction::down;
            if (heroView.get<Position>(h).c.x + heroView.get<Sprite>(h).spriteSheet->getSize().w <= view.get<Position>(e).c.x)
                view.get<Movable>(e).direction = Direction::left;
            view.get<Character>(e).dialoguing = true;
            return view.get<Character>(e).name;
        }
    }
    return "";
}

void Interaction::finishDialog(entt::registry &reg)
{
    const auto view = reg.view<Character, Position, Sprite, Movable>();
    for (const entt::entity e : view)
    {
        view.get<Character>(e).dialoguing = false;
    }
}