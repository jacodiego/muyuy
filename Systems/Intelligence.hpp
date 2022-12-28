#pragma once

#include "../Components/Hero.hpp"
#include "../Components/Character.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/Position.hpp"
#include "../Components/Movable.hpp"
#include "../Components/Collisionable.hpp"
#include "../Components/Walker.hpp"
#include "../Gui/Map.hpp"
#include "../Utilities/Grid.hpp"
#include "../Utilities/PathFinder.hpp"
#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <random>

class Intelligence
{
public:
    static void walkers(entt::registry &, Map *);
};
