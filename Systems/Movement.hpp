#pragma once

#include "../Components/Hero.hpp"
#include "../Components/Character.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/Position.hpp"
#include "../Components/Movable.hpp"
#include "../Components/Collisionable.hpp"
#include "../Components/Walker.hpp"
#include "../Core/Graphics.hpp"
#include "../Core/Util.hpp"
#include "../Gui/Map.hpp"
#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>
#include <SDL2/SDL.h>
#include "iostream"
#include "iomanip"

class Movement
{
public:
    static void hero(entt::registry &, SDL_Rect &, Map *);
    static void walkers(entt::registry &, Map *);
};
