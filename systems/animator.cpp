#include "animator.hpp"

namespace muyuy::ecs::systems
{

    void Animator::walkers(entt::registry &reg)
    {
        auto view = reg.view<components::Animation, components::Character, components::Movement, components::Position, components::Rotation, components::Sprite>();
        auto time_elapsed = system::systemManager->getUpdateTime();
        for (auto e : view)
        {
            auto &animation = view.get<components::Animation>(e);
            auto &frames = view.get<components::Animation>(e).state_map.at(view.get<components::Movement>(e).state + "_" + view.get<components::Rotation>(e).last_direction);
            if (animation.last_state != view.get<components::Movement>(e).state + "_" + view.get<components::Rotation>(e).last_direction)
            {
                animation.last_update = 0;
                animation.current_index = 0;
            }
            else
            {
                if (animation.last_update > animation.current_frame.second)
                {
                    animation.last_update = 0;
                    if (animation.current_index == frames.size() - 1)
                        animation.current_index = 0;
                    else
                        animation.current_index++;
                }
            }
            animation.current_frame = frames[animation.current_index];
            animation.last_update += time_elapsed;
            animation.last_state = view.get<components::Movement>(e).state + "_" + view.get<components::Rotation>(e).last_direction;
        }
    }

    void Animator::objects(entt::registry &reg)
    {
        auto view = reg.view<components::Animation, components::Position, components::Sprite>(entt::exclude<components::Character>);
        auto time_elapsed = system::systemManager->getUpdateTime();
        for (auto e : view)
        {
            auto &animation = view.get<components::Animation>(e);
            auto &frames = view.get<components::Animation>(e).state_map.at("animation");
            if (animation.last_state != "animation")
            {
                animation.last_update = 0;
                animation.current_index = 0;
                animation.last_state = "animation";
            }
            else
            {
                if (animation.last_update > animation.current_frame.second)
                {
                    animation.last_update = 0;
                    if (animation.current_index == frames.size() - 1)
                        animation.current_index = 0;
                    else
                        animation.current_index++;
                }
            }
            animation.current_frame = frames[animation.current_index];
            animation.last_update += time_elapsed;
        }
    }

}