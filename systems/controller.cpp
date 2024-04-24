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
            movement.northward = input::inputManager->upState();
            movement.eastward = input::inputManager->rightState();
            movement.southward = input::inputManager->downState();
            movement.westward = input::inputManager->leftState();
            movement.state = movement.northward || movement.eastward || movement.southward || movement.westward ? 
                (input::inputManager->confirmState() ? "running" : "walking") : "idle";
            
            if (movement.northward && !movement.eastward && !movement.southward && !movement.westward)
            {
                rotation.direction = "north";
            } 
            else if (movement.southward && !movement.eastward && !movement.northward && !movement.westward)
            {
                rotation.direction = "south";
            } 
            else if (movement.eastward && !movement.northward && !movement.southward && !movement.westward)
            {
                rotation.direction = "east";
            }
            else if (movement.westward && !movement.eastward && !movement.southward && !movement.northward)
            {
                rotation.direction = "west";
            }
        }
    }

}