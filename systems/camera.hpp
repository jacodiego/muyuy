#pragma once

#include "components/character.hpp"
#include "components/collisionable.hpp"
#include "components/movement.hpp"
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

    class Camera
    {
    public:
        static void followCharacter(entt::registry &, Rect &, map::Map *);

    private:
        static void setCameraOn(Rect &, map::Map *, uint16_t, uint16_t);
    };

}
