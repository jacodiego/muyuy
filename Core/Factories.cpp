#include "Factories.hpp"
#include "../Components/Hero.hpp"
#include "../Components/Position.hpp"
#include "../Components/Sprite.hpp"
#include "../Components/Movable.hpp"
#include "../Components/Openable.hpp"
#include "../Components/Item.hpp"
#include "../Components/Character.hpp"
#include "../Components/Collisionable.hpp"
#include "../Components/Walker.hpp"
#include "../Utilities/Coordinate.hpp"
#include <entt/entity/registry.hpp>

entt::entity Factories::makeHero(entt::registry &reg, SpriteSheet *heroSprites, Coordinate c)
{
    const entt::entity e = reg.create();
    reg.emplace<Hero>(e);
    reg.emplace<Movable>(e);
    reg.emplace<Position>(e, c);
    reg.emplace<Sprite>(e, heroSprites);
    return e;
}

entt::entity Factories::makeCharacter(entt::registry &reg, SpriteSheet *heroSprites, Coordinate c, std::string name)
{
    const entt::entity e = reg.create();
    reg.emplace<Character>(e, name);
    reg.emplace<Movable>(e);
    reg.emplace<Walker>(e);
    reg.emplace<Collisionable>(e);
    reg.emplace<Position>(e, c);
    reg.emplace<Sprite>(e, heroSprites);
    return e;
}

entt::entity Factories::makeTreasure(entt::registry &reg, SpriteSheet *heroSprites, Coordinate c, char *itemId)
{
    const entt::entity e = reg.create();
    reg.emplace<Openable>(e);
    reg.emplace<Collisionable>(e);
    reg.emplace<Position>(e, c);
    reg.emplace<Item>(e, itemId);
    reg.emplace<Sprite>(e, heroSprites);
    return e;
}