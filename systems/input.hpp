#pragma once

#include "components/character.hpp"
#include "components/move.hpp"
#include "components/position.hpp"
#include "components/state.hpp"
#include "engine/input.hpp"
#include "utils/rect.hpp"

#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>

namespace muyuy::ecs::systems
{

    class Input
    {
    public:
        static void move(entt::registry &);
    };

}
