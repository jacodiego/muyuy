#pragma once

namespace muyuy::ecs::components
{
    struct Move
    {
        int velocity = 1;
        bool northward = false;
        bool eastward = false;
        bool southward = false;
        bool westward = false;
        bool running = false;
    };
}