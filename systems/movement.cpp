#include "movement.hpp"

namespace muyuy::ecs::systems
{

    void Movement::character(entt::registry &reg, Rect &camera, int mapWidth, int mapHeight)
    {
        const auto view = reg.view<components::Character, components::Movement, components::Position, components::Sprite, components::Rotation>();
        for (const entt::entity e : view)
        {
            int moveX = 0;
            int moveY = 0;
            if (view.get<components::Movement>(e).state != "idle")
            {
                int vel = view.get<components::Movement>(e).state == "running" ? view.get<components::Movement>(e).velocity * 2 : view.get<components::Movement>(e).velocity;
                if (view.get<components::Rotation>(e).northward)
                {
                    moveY = vel * -1;
                }
                if (view.get<components::Rotation>(e).eastward)
                {
                    moveX = vel;
                }
                if (view.get<components::Rotation>(e).southward)
                {
                    moveY = vel;
                }
                if (view.get<components::Rotation>(e).westward)
                {
                    moveX = vel * -1;
                }

                view.get<components::Position>(e).x += moveX;
                view.get<components::Position>(e).y += moveY;

                if (view.get<components::Position>(e).x < 0 ||
                    view.get<components::Position>(e).x + view.get<components::Sprite>(e).width > mapWidth)
                    view.get<components::Position>(e).x -= moveX;
                if (view.get<components::Position>(e).y < 0 ||
                    view.get<components::Position>(e).y + view.get<components::Sprite>(e).height > mapHeight)
                    view.get<components::Position>(e).y -= moveY;

                camera.x = (view.get<components::Position>(e).x + view.get<components::Sprite>(e).width / 2) - video::videoManager->getWindowWidth() / 2;
                camera.y = (view.get<components::Position>(e).y + view.get<components::Sprite>(e).height / 2) - video::videoManager->getWindowHeight() / 2;

                if (camera.x < 0)
                {
                    camera.x = 0;
                }
                if (camera.y < 0)
                {
                    camera.y = 0;
                }
                if (camera.x > mapWidth - camera.width)
                {
                    camera.x = mapWidth - camera.width;
                }
                if (camera.y > mapHeight - camera.height)
                {
                    camera.y = mapHeight - camera.height;
                }
            }
        }
    }

}