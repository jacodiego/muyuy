#pragma once

#include "device.hpp"
#include "renderer.hpp"
#include "swapchain.hpp"
#include "buffer.hpp"

namespace muyuy::video
{

    class Renderer;
    class TextureArray
    {
    public:
        TextureArray(Device &, std::vector<TextureMap>, vk::DescriptorPool, vk::DescriptorSetLayout);
        ~TextureArray();
        vk::DescriptorSet getDescriptorSet(int i) { return descriptorSets[i]; };

    private:
        void createTextureSampler();
        void createDescriptorSets(vk::DescriptorPool, vk::DescriptorSetLayout);

    private:
        Device &device;
        Buffer buffer{device};
        std::vector<TextureMap> _texture_maps;
        std::vector<vk::DescriptorSet> descriptorSets;
        vk::Sampler _sampler;
    };

}
