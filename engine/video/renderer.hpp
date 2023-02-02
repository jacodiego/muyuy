#pragma once

#include "device.hpp"
#include "swapchain.hpp"
#include "texture.hpp"
#include "buffer.hpp"

#include "utils/file.hpp"

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <map>
#include <algorithm>
#include <glm/glm.hpp>

namespace muyuy::video
{

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 texCoord;

        static vk::VertexInputBindingDescription getBindingDescription()
        {
            vk::VertexInputBindingDescription bindingDescription{
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = vk::VertexInputRate::eVertex};

            return bindingDescription;
        }

        static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions()
        {
            std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = vk::Format::eR32G32Sfloat;
            attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };

    enum class pipelineLayoutTypes
    {
        Sampler,
        UboSampler
    };

    enum class pipelineTypes
    {
        GraphicSampler,
        GraphicUboSampler,
    };

    enum class shaderModuleTypes
    {
        VertexSampler,
        FragmentSampler,
        VertexUboSampler,
        FragmentUboSampler
    };

    enum class descriptorTypes
    {
        Sampler,
        Ubo,
        UboSampler
    };

    class Device;
    class Swapchain;
    class Texture;
    class Renderer
    {

        // const std::vector<Vertex> vertices = {
        //     {{-1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 0.5f}, {1.0f, 0.0f}},
        //     {{1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 0.5f}, {0.0f, 0.0f}},
        //     {{1.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
        //     {{-1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 0.5f}, {1.0f, 1.0f}}};


        const std::vector<Vertex> vertices = {
            {{-1.0f, -1.0f}, {1.0f, 0.0f}},
            {{1.0f, -1.0f}, {0.0f, 0.0f}},
            {{1.0f, 1.0f}, {0.0f, 1.0f}},
            {{-1.0f, 1.0f}, {1.0f, 1.0f}}};

        const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0};

    public:
        explicit Renderer(Device &);
        void initialize(SDL_Event *);
        void draw();
        void resize() { framebufferResized = true; };
        void destroy();
        vk::DescriptorPool getDescriptorPool(descriptorTypes dt) { return descriptorPool.at(dt); };
        vk::DescriptorSetLayout getDescriptorSetLayout(descriptorTypes dt) { return descriptorSetLayouts.at(dt); };
        void addDrawTexture(Texture *);
        void removeDrawTexture(Texture *);

    private:
        void recreateSwapChain();
        void createCommandBuffers();
        void createPipelineLayout(pipelineLayoutTypes, vk::DescriptorSetLayout *);
        void createPipeline(pipelineTypes, vk::PipelineLayout, vk::PipelineVertexInputStateCreateInfo, vk::ShaderModule, vk::ShaderModule);
        void createShaderModule(shaderModuleTypes, std::string);
        void createDescriptorSetLayout(descriptorTypes, std::vector<vk::DescriptorSetLayoutBinding>);
        void createDescriptorPool(descriptorTypes, std::vector<vk::DescriptorPoolSize>);
        void recordCommandBuffer(vk::CommandBuffer, uint32_t);

    private:
        Device &device;
        SDL_Event *event;
        Swapchain swapchain{device};
        Buffer buffer{device};
        std::vector<vk::CommandBuffer> commandBuffers;
        std::map<pipelineLayoutTypes, vk::PipelineLayout> pipelineLayouts;
        std::map<pipelineTypes, vk::Pipeline> pipelines;
        std::map<shaderModuleTypes, vk::ShaderModule> shaders;
        std::map<descriptorTypes, vk::DescriptorSetLayout> descriptorSetLayouts;
        std::map<descriptorTypes, vk::DescriptorPool> descriptorPool;
        uint32_t currentFrame = 0;
        bool framebufferResized = false;

        vk::Buffer vertexBuffer;
        vk::DeviceMemory vertexBufferMemory;
        vk::Buffer indexBuffer;
        vk::DeviceMemory indexBufferMemory;

        std::vector<Texture *> _draw_textures;
    };

}
