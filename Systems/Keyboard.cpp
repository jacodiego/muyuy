#include "Keyboard.hpp"

namespace
{

  Direction readDirection(const SDL_Scancode key)
  {
    switch (key)
    {
    case SDL_SCANCODE_W:
    case SDL_SCANCODE_UP:
      return Direction::up;
    case SDL_SCANCODE_D:
    case SDL_SCANCODE_RIGHT:
      return Direction::right;
    case SDL_SCANCODE_S:
    case SDL_SCANCODE_DOWN:
      return Direction::down;
    case SDL_SCANCODE_A:
    case SDL_SCANCODE_LEFT:
      return Direction::left;
    default:
      return Direction::none;
    }
  }

}

bool Keyboard::pressed(entt::registry &reg, const SDL_Scancode key)
{
  const Direction dir = readDirection(key);
  if (dir == Direction::none)
  {
    return false;
  }
  auto view = reg.view<Hero, Movable>();
  for (const entt::entity e : view)
  {
    view.get<Movable>(e).direction = dir;
    if (dir == Direction::up)
    {
      view.get<Movable>(e).movingUp = true;
      view.get<Movable>(e).movingDown = false;
    }
    if (dir == Direction::down)
    {
      view.get<Movable>(e).movingDown = true;
      view.get<Movable>(e).movingUp = false;
    }
    if (dir == Direction::left)
    {
      view.get<Movable>(e).movingLeft = true;
      view.get<Movable>(e).movingRight = false;
    }
    if (dir == Direction::right)
    {
      view.get<Movable>(e).movingLeft = false;
      view.get<Movable>(e).movingRight = true;
    }
  }
  return true;
}

bool Keyboard::released(entt::registry &reg, const SDL_Scancode key)
{
  const Direction dir = readDirection(key);
  if (dir == Direction::none)
  {
    return false;
  }
  auto view = reg.view<Hero, Movable>();
  for (const entt::entity e : view)
  {

    view.get<Movable>(e).direction = dir;
    if (dir == Direction::up)
    {
      view.get<Movable>(e).movingUp = false;
      if (view.get<Movable>(e).movingLeft)
        view.get<Movable>(e).direction = Direction::left;
      if (view.get<Movable>(e).movingRight)
        view.get<Movable>(e).direction = Direction::right;
      if (view.get<Movable>(e).movingDown)
        view.get<Movable>(e).direction = Direction::down;
    }
    if (dir == Direction::down)
    {
      view.get<Movable>(e).movingDown = false;
      if (view.get<Movable>(e).movingLeft)
        view.get<Movable>(e).direction = Direction::left;
      if (view.get<Movable>(e).movingRight)
        view.get<Movable>(e).direction = Direction::right;
      if (view.get<Movable>(e).movingUp)
        view.get<Movable>(e).direction = Direction::up;
    }
    if (dir == Direction::left)
    {
      view.get<Movable>(e).movingLeft = false;
      if (view.get<Movable>(e).movingUp)
        view.get<Movable>(e).direction = Direction::up;
      if (view.get<Movable>(e).movingDown)
        view.get<Movable>(e).direction = Direction::down;
      if (view.get<Movable>(e).movingRight)
        view.get<Movable>(e).direction = Direction::right;
    }
    if (dir == Direction::right)
    {
      view.get<Movable>(e).movingRight = false;
      if (view.get<Movable>(e).movingUp)
        view.get<Movable>(e).direction = Direction::up;
      if (view.get<Movable>(e).movingDown)
        view.get<Movable>(e).direction = Direction::down;
      if (view.get<Movable>(e).movingLeft)
        view.get<Movable>(e).direction = Direction::left;
    }
  }
  return true;
}
