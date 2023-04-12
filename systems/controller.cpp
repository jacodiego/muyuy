#include "controller.hpp"

namespace muyuy::ecs::systems
{

    void Controller::move(entt::registry &reg)
    {
        const auto view = reg.view<components::Character, components::Movement, components::Position, components::Rotation>();
        for (auto e : view)
        {
            auto &rotation = view.get<components::Rotation>(e);
            auto &movement = view.get<components::Movement>(e);
            rotation.northward = input::inputManager->upState();
            rotation.eastward = input::inputManager->rightState();
            rotation.southward = input::inputManager->downState();
            rotation.westward = input::inputManager->leftState();
            movement.state = rotation.northward || rotation.eastward || rotation.southward || rotation.westward ? "walking" : "idle";
            if (input::inputManager->upPress())
            {
                rotation.last_direction = "north";
            }
            if (input::inputManager->rightPress())
            {
                rotation.last_direction = "east";
            }
            if (input::inputManager->downPress())
            {
                rotation.last_direction = "south";
            }
            if (input::inputManager->leftPress())
            {
                rotation.last_direction = "west";
            }
            if (input::inputManager->confirmState() && movement.state == "walking")
            {
                movement.state = "running";
            }
        }
    }

}