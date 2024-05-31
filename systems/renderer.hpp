#pragma once

#include "components/animation.hpp"
#include "components/character.hpp"
#include "components/npc.hpp"
#include "components/movement.hpp"
#include "components/openable.hpp"
#include "components/position.hpp"
#include "components/rotation.hpp"
#include "components/sprite.hpp"
#include "components/walker.hpp"
#include "engine/video/video.hpp"
#include "utils/rect.hpp"

#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>

namespace muyuy::ecs::systems
{

    class Renderer
    {
    public:
        static void characters(entt::registry &, Rect &);
        static void walkers(entt::registry &, Rect &);
        static void objects(entt::registry &, Rect &);
        static void openables(entt::registry &, Rect &);
    };

}
