#pragma once

#include "../Components/Openable.hpp"
#include "../Components/Hero.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/Position.hpp"
#include "../Components/Movable.hpp"
#include "../Components/Character.hpp"
#include "../Core/Util.hpp"
#include "../Utilities/Direction.hpp"
#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>
#include <SDL2/SDL.h>
#include <string>

class Interaction
{
public:
    static void open(entt::registry &);
    static std::string talk(entt::registry &);
    static void finishDialog(entt::registry &);
};
