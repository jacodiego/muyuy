#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <entt/entity/registry.hpp>

#include "components/animation.hpp"
#include "components/character.hpp"
#include "components/collisionable.hpp"
#include "components/movement.hpp"
#include "components/npc.hpp"
#include "components/position.hpp"
#include "components/rotation.hpp"
#include "components/sprite.hpp"

namespace muyuy::utils
{
    void updateEntity(entt::registry *, entt::entity, sol::table);
}