#pragma once

#include "components/character.hpp"
#include "components/move.hpp"
#include "components/position.hpp"
#include "components/state.hpp"
#include "engine/video/video.hpp"
#include "utils/rect.hpp"

#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>

namespace muyuy::ecs::systems
{

    class Drawer
    {
    public:
        static void walkers(entt::registry &, int, Rect &);
    };

}
