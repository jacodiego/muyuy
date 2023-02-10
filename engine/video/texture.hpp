#pragma once

#include "device.hpp"
#include "renderer.hpp"
#include "buffer.hpp"
#include "swapchain.hpp"

#include "texture_base.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace muyuy::video
{

    struct UniformBufferObject
    {
        glm::float32_t alpha;
        glm::float32_t scale;
    };

    class Renderer;
    class Texture : public TextureBase
    {
    public:
        Texture(Device &, Renderer *);
        void load(const char *, vk::DescriptorPool, vk::DescriptorSetLayout);
        void draw(ScreenPosition, int, int, int, int, float alpha = 0.5f, float scale = 1.0f);
        void draw(int, int, int, int, int, int, float alpha = 0.5f, float scale = 1.0f);
        vk::DescriptorSet getDescriptorSet(int i) override { return descriptorSets[i]; };
        int getWidth() override { return width; };
        int getHeight() override { return height; };
        void updateUniformBuffer(uint32_t, float, float);
        TextureWindow getTextureWindow() override;

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
        std::vector<vk::Buffer> uniformBuffers;
        std::vector<vk::DeviceMemory> uniformBuffersMemory;
        std::vector<void *> uniformBuffersMapped;
    };

}
