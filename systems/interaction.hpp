#pragma once

#include "components/character.hpp"
#include "components/collisionable.hpp"
#include "components/movement.hpp"
#include "components/openable.hpp"
#include "components/position.hpp"
#include "components/rotation.hpp"
#include "components/sprite.hpp"
#include "engine/video/video.hpp"
#include "screens/map/map.hpp"
#include "utils/rect.hpp"

#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>

namespace muyuy::ecs::systems
{

    class Interaction
    {
    public:
        static void open(entt::registry &, entt::registry &);
        /* static std::string talk(entt::registry &);
        static void finishDialog(entt::registry &); */
    };

}