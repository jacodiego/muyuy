#include "drawer.hpp"

namespace muyuy::ecs::systems
{

    void Drawer::walkers(entt::registry &reg, const int frame, Rect &camera)
    {
        const auto view = reg.view<components::Character, components::Move, components::Position, components::State>();
        for (const entt::entity e : view)
        {
            // view.get<components::Character>(e).getName() + "_" +view.get<components::State>(e).get()
            // video::videoManager->getAnimation(view.get<components::Character>(e).getName() + "_" + view.get<components::State>(e).get())->draw("south");
            video::videoManager->drawAnimation("kumelen_" + view.get<components::State>(e).name, view.get<components::State>(e).description, view.get<components::Position>(e).x, view.get<components::Position>(e).y);
        }
    }

}