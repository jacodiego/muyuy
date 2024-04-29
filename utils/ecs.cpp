#include "ecs.hpp"

namespace muyuy::utils
{
    void updateEntity(entt::registry *reg, entt::entity ent, sol::table components)
    {
        for (const auto &component : components)
        {
            sol::object component_key = component.first;
            sol::table component_value = component.second.as<sol::table>();

            if (component_key.as<std::string>() == "character")
                reg->emplace_or_replace<ecs::components::Character>(ent,
                                                                    component_value.get_or<std::string>("name", "Unknown"),
                                                                    component_value.get<bool>("available"),
                                                                    component_value.get<bool>("active"));
            if (component_key.as<std::string>() == "npc")
                reg->emplace_or_replace<ecs::components::Npc>(ent,
                                                              component_value.get_or<std::string>("name", "Unknown"));
            if (component_key.as<std::string>() == "sprite")
                reg->emplace_or_replace<ecs::components::Sprite>(ent,
                                                                 component_value.get<uint16_t>("width"),
                                                                 component_value.get<uint16_t>("height"),
                                                                 component_value.get<uint16_t>("rows"),
                                                                 component_value.get<uint16_t>("cols"),
                                                                 video::videoManager->createImage(component_value.get<std::string>("image_filename").c_str()));
            if (component_key.as<std::string>() == "movement")
                reg->emplace_or_replace<ecs::components::Movement>(ent);
            if (component_key.as<std::string>() == "position")
                reg->emplace_or_replace<ecs::components::Position>(ent, component_value.get_or<uint16_t>("x", 0), component_value.get_or<uint16_t>("y", 0));
            if (component_key.as<std::string>() == "rotation")
                reg->emplace_or_replace<ecs::components::Rotation>(ent, component_value.get<char>("direction"));
            if (component_key.as<std::string>() == "collisionable")
                reg->emplace_or_replace<ecs::components::Collisionable>(ent);

            if (component_key.as<std::string>() == "animation")
            {
                std::unordered_map<std::string, std::vector<std::pair<uint16_t, uint16_t>>> state_map;
                for (const auto &properties : component_value)
                {
                    sol::object property_key = properties.first;
                    sol::table property_value = properties.second.as<sol::table>();
                    std::vector<std::pair<uint16_t, uint16_t>> frames;

                    for (const auto &frame : property_value)
                    {
                        sol::table frame_value = frame.second.as<sol::table>();
                        frames.push_back(std::make_pair(frame_value.get<uint16_t>("id"), frame_value.get<uint16_t>("duration")));
                    }
                    state_map.insert(std::make_pair(property_key.as<std::string>(), frames));
                }
                reg->emplace_or_replace<ecs::components::Animation>(ent, state_map);
            }
        }
    }
}