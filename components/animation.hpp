#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>


namespace muyuy::ecs::components
{
    struct Animation
    {
        std::unordered_map<std::string, std::vector<std::pair<uint16_t, uint16_t>>> state_map;
        uint32_t last_update = 0;
        uint16_t current_index = 0;
        std::pair<uint16_t, uint16_t> current_frame;
        std::string last_state;
    };
}