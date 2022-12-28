#pragma once

#include <entt/entity/fwd.hpp>
#include <SDL2/SDL.h>

class Render
{
public:
    static void movable(entt::registry &, int, SDL_Rect &);
    static void treasures(entt::registry &, SDL_Rect &);
};
