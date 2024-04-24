#pragma once

#include <string>

namespace muyuy::ecs::components
{
    struct Movement
    {
        int velocity = 1;
        std::string state = "idle";
        bool northward = false;
        bool eastward = false;
        bool southward = false;
        bool westward = false;
    };
}