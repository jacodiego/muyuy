#pragma ocne

#include "../Components/Hero.hpp"
#include "../Components/Movable.hpp"
#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>
#include <iostream>
#include <cstdint>
#include <SDL2/SDL_scancode.h>

class Keyboard
{
public:
    static bool pressed(entt::registry &, SDL_Scancode);
    static bool released(entt::registry &, SDL_Scancode);
};
