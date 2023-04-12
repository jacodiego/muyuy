#pragma once

#include <string>

namespace muyuy::ecs::components
{
    struct Rotation
    {
        std::string last_direction = "south";
        bool northward = false;
        bool eastward = false;
        bool southward = false;
        bool westward = false;
    };
}