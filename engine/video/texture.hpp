#pragma once

#include "device.hpp"
#include "buffer.hpp"

namespace muyuy::video
{

    class Texture
    {
    public:
        Texture(Device &);
        void load(const char *);
        vk::ImageView getImageView() { return textureImageView; };
        vk::Sampler getSampler() { return textureSampler; };

    private:
        void createImage(uint32_t, uint32_t, vk::Format, vk::ImageTiling, vk::ImageUsageFlags, vk::MemoryPropertyFlags);
        void transitionImageLayout(vk::Format, vk::ImageLayout, vk::ImageLayout);
        void createTextureImageView();
        void createTextureSampler();

    private:
        Device &device;
        Buffer buffer{device};
        vk::Image textureImage;
        vk::DeviceMemory textureImageMemory;
        vk::ImageView textureImageView;
        vk::Sampler textureSampler;
    };

}
