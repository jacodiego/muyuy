#include "input.hpp"

namespace muyuy::ecs::systems
{

    void Input::move(entt::registry &reg)
    {
        const auto view = reg.view<components::Character, components::Move, components::Position, components::State>();
        for (const entt::entity e : view)
        {
            view.get<components::Move>(e).running = input::inputManager->confirmState();
            view.get<components::Move>(e).northward = input::inputManager->upState();
            view.get<components::Move>(e).eastward = input::inputManager->rightState();
            view.get<components::Move>(e).southward = input::inputManager->downState();
            view.get<components::Move>(e).westward = input::inputManager->leftState();
        }
    }

}