#pragma once

#include <string>

namespace muyuy::ecs::components
{
    struct Movement
    {
        int velocity = 2;
        std::string state = "idle";
    };
}