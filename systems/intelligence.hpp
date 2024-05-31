#pragma once

#include "components/character.hpp"
#include "components/collisionable.hpp"
#include "components/movement.hpp"
#include "components/npc.hpp"
#include "components/position.hpp"
#include "components/rotation.hpp"
#include "components/sprite.hpp"
#include "components/walker.hpp"
#include "screens/map/map.hpp"
#include "utils/grid.hpp"
#include "utils/path_finder.hpp"

#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>
#include <SDL2/SDL.h>
#include <iostream>
#include <random>

namespace muyuy::ecs::systems
{
    class Intelligence
    {
    public:
        static void walkers(entt::registry &, map::Map *);
    };
}