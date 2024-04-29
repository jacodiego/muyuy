#include "renderer.hpp"

namespace muyuy::ecs::systems
{
    void Renderer::walkers(entt::registry &reg, Rect &camera)
    {
        auto view = reg.view<components::Animation, components::Character, components::Movement, components::Position, components::Rotation, components::Sprite>();
        for (auto e : view)
        {
            auto &character = view.get<components::Character>(e);
            if (character.active)
            {
                int id = view.get<components::Animation>(e).current_frame.first - 1;
                auto offset_x = view.get<components::Sprite>(e).width * ((id % view.get<components::Sprite>(e).cols) - 1);
                auto offset_y = view.get<components::Sprite>(e).height * (id / view.get<components::Sprite>(e).cols);
                view.get<components::Sprite>(e).texture->draw(view.get<components::Position>(e).x - camera.x, view.get<components::Position>(e).y - camera.y, offset_x, offset_y, view.get<components::Sprite>(e).width, view.get<components::Sprite>(e).height, 1, 1, 1);
            }
        }
    }

    void Renderer::npc(entt::registry &reg, Rect &camera)
    {
        auto view = reg.view<components::Npc, components::Position, components::Rotation, components::Sprite>();
        for (auto e : view)
        {
            int id = 5;
            auto offset_x = view.get<components::Sprite>(e).width * ((id % view.get<components::Sprite>(e).cols) - 1);
            auto offset_y = view.get<components::Sprite>(e).height * (id / view.get<components::Sprite>(e).cols);
            view.get<components::Sprite>(e).texture->draw(view.get<components::Position>(e).x - camera.x, view.get<components::Position>(e).y - camera.y, offset_x, offset_y, view.get<components::Sprite>(e).width, view.get<components::Sprite>(e).height, 1, 1, 1);
        }
    }

    void Renderer::objects(entt::registry &reg, Rect &camera)
    {
        auto view = reg.view<components::Animation, components::Position, components::Sprite>();
        for (auto e : view)
        {
            int id = view.get<components::Animation>(e).current_frame.first - 1;
            auto offset_x = view.get<components::Sprite>(e).width * ((id % view.get<components::Sprite>(e).cols) - 1);
            auto offset_y = view.get<components::Sprite>(e).height * (id / view.get<components::Sprite>(e).cols);
            view.get<components::Sprite>(e).texture->draw(view.get<components::Position>(e).x - camera.x, view.get<components::Position>(e).y - camera.y, offset_x, offset_y, view.get<components::Sprite>(e).width, view.get<components::Sprite>(e).height, 1, 1, 1);
        }
    }

    void Renderer::openables(entt::registry &reg, Rect &camera)
    {
        auto view = reg.view<components::Openable, components::Position, components::Sprite>();
        for (auto e : view)
        {
            int id = view.get<components::Openable>(e).isOpen ? 0 : 1;
            auto offset_x = view.get<components::Sprite>(e).width * ((id % view.get<components::Sprite>(e).cols) - 1);
            auto offset_y = view.get<components::Sprite>(e).height * (id / view.get<components::Sprite>(e).cols);
            view.get<components::Sprite>(e).texture->draw(view.get<components::Position>(e).x - camera.x, view.get<components::Position>(e).y - camera.y, offset_x, offset_y, view.get<components::Sprite>(e).width, view.get<components::Sprite>(e).height, 1, 1, 1);
        }
    }
}