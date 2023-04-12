#pragma once

#include "engine/video/video.hpp"
#include <cstdint>

namespace muyuy::ecs::components
{
    struct Sprite
    {
        uint16_t width;
        uint16_t height;
        uint16_t rows;
        uint16_t cols;
        video::Texture *texture;
    };
}