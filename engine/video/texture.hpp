#pragma once

#include "device.hpp"
#include "renderer.hpp"
#include "buffer.hpp"
#include "swapchain.hpp"
#include "color.hpp"

#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace muyuy::video
{

    struct UniformBufferObject
    {
        glm::float32_t alpha;
        glm::float32_t scale;
        glm::float32_t multiplyColor;
    };

    class Renderer;
    class Texture
    {
    public:
        Texture(Device &, Renderer *);
        void initialize(int, int, vk::Format, vk::DescriptorPool, vk::DescriptorSetLayout);
        void addPixels(uint8_t *, uint32_t, uint32_t, uint32_t, uint32_t);
        void loadFromImage(const char *, vk::DescriptorPool, vk::DescriptorSetLayout);
        void draw(ScreenPosition, int, int, int, int, float alpha, float scale, float multiplyColor);
        void draw(int x, int y, int offset_x, int offset_y, int width, int height, float alpha, float scale, float multiplyColor);
        vk::DescriptorSet getDescriptorSet(int i) { return descriptorSets[i]; };
        int getWidth() { return width; };
        int getHeight() { return height; };
        void updateUniformBuffer(uint32_t, float, float, float);
        glm::vec3 getColor() { return _color.getVec3(); };
        void setColor(Color color) { _color = color; };

    private:
        void createImage(vk::Format, vk::ImageTiling, vk::ImageUsageFlags, vk::MemoryPropertyFlags);
        void transitionImageLayout(vk::ImageLayout, vk::ImageLayout);
        void createTextureImageView(vk::Format);
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
        Color _color;
    };

}
