#pragma once

#include "components/animation.hpp"
#include "components/character.hpp"
#include "components/movement.hpp"
#include "components/position.hpp"
#include "components/rotation.hpp"
#include "components/sprite.hpp"
#include "engine/video/video.hpp"
#include "utils/rect.hpp"

#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>

namespace muyuy::ecs::systems
{

    class Renderer
    {
    public:
        static void walkers(entt::registry &, Rect &);
        static void objects(entt::registry &, Rect &);
    };

}
