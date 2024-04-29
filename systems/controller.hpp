#pragma once

#include "components/character.hpp"
#include "components/movement.hpp"
#include "components/openable.hpp"
#include "components/position.hpp"
#include "components/rotation.hpp"
#include "engine/input.hpp"
#include "utils/rect.hpp"

#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>

namespace muyuy::ecs::systems
{

    class Controller
    {
    public:
        static void move(entt::registry &);
    };

}
