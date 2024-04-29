#include "camera.hpp"

namespace muyuy::ecs::systems
{

    void Camera::followCharacter(entt::registry &reg, Rect &camera, map::Map *map)
    {
        const auto view = reg.view<components::Character, components::Movement, components::Position, components::Sprite, components::Rotation>();
        for (const entt::entity e : view)
        {
            auto &character = view.get<components::Character>(e);
            if (character.active)
            {
                setCameraOn(camera, map, view.get<components::Position>(e).x + view.get<components::Sprite>(e).width / 2, view.get<components::Position>(e).y + view.get<components::Sprite>(e).height / 2);
            }
        }
    }

    void Camera::setCameraOn(Rect &camera, map::Map *map, uint16_t x, uint16_t y)
    {
        camera.x = x - video::videoManager->getWindowWidth() / 2;
        camera.y = y - video::videoManager->getWindowHeight() / 2;

        if (camera.x < 0)
        {
            camera.x = 0;
        }
        if (camera.y < 0)
        {
            camera.y = 0;
        }
        if (camera.x > map->getSize().width - camera.width)
        {
            camera.x = map->getSize().width - camera.width;
        }
        if (camera.y > map->getSize().height - camera.height)
        {
            camera.y = map->getSize().height - camera.height;
        }
    }
}