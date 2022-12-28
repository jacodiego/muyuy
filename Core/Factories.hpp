#pragma once

#include "../Gui/SpriteSheet.hpp"
#include <entt/entity/fwd.hpp>
#include <string>

class Factories
{
public:
    static entt::entity makeHero(entt::registry &, SpriteSheet *heroSprites, Coordinate);
    static entt::entity makeCharacter(entt::registry &, SpriteSheet *heroSprites, Coordinate, std::string);
    static entt::entity makeTreasure(entt::registry &, SpriteSheet *heroSprites, Coordinate, char *);
};