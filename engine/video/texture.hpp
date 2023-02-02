#pragma once

#include "device.hpp"
#include "renderer.hpp"
#include "buffer.hpp"
#include "swapchain.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace muyuy::video
{

    struct UniformBufferObject
    {
        glm::float32_t alpha;
    };

    class Renderer;
    class Texture
    {
    public:
        Texture(Device &, Renderer *);
        void load(const char *, vk::DescriptorPool, vk::DescriptorSetLayout);
        void draw();
        void undraw();
        vk::DescriptorSet getDescriptorSet(int i) { return descriptorSets[i]; };
        int getWidth() { return width; };
        int getHeight() { return height; };
        float getAlpha() { return alpha; };
        void setAlpha(float a) { alpha = a; };
        void updateUniformBuffer(uint32_t);

    private:
        void createImage(vk::Format, vk::ImageTiling, vk::ImageUsageFlags, vk::MemoryPropertyFlags);
        void transitionImageLayout(vk::Format, vk::ImageLayout, vk::ImageLayout);
        void createTextureImageView();
        void createTextureSampler();
        void createDescriptorSets(vk::DescriptorPool, vk::DescriptorSetLayout);
        void createUniformBuffers();

    private:
        Device &device;
        Renderer *renderer;
        Buffer buffer{device};
        vk::Image textureImage;
        vk::DeviceMemory textureImageMemory;
        vk::ImageView textureImageView;
        vk::Sampler textureSampler;
        std::vector<vk::DescriptorSet> descriptorSets;
        int width, height;
        float alpha = 1;

        std::vector<vk::Buffer> uniformBuffers;
        std::vector<vk::DeviceMemory> uniformBuffersMemory;
        std::vector<void *> uniformBuffersMapped;
    };

}
