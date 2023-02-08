#pragma once

#include <vulkan/vulkan.hpp>

namespace muyuy::video
{

    struct TextureWindow
    {
        float x0;
        float y0;
        float x1;
        float y1;
    };

    struct TextureBase
    {
        virtual vk::DescriptorSet getDescriptorSet(int) = 0;
        virtual TextureWindow getTextureWindow() = 0;
        virtual int getWidth() = 0;
        virtual int getHeight() = 0;
    };

}
