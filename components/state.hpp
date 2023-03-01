#pragma once

#include <string>

namespace muyuy::ecs::components
{
    struct State
    {
        std::string name = "none";
        std::string description = "none";
    };
}