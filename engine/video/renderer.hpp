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
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace muyuy::video
{

    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
        glm::vec2 texCoord;

        static vk::VertexInputBindingDescription getBindingDescription()
        {
            vk::VertexInputBindingDescription bindingDescription{
                .binding = 0,
                .stride = sizeof(Vertex),
                .inputRate = vk::VertexInputRate::eVertex};

            return bindingDescription;
        }

        static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions()
        {
            std::array<vk::VertexInputAttributeDescription, 3> attributeDescriptions{};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
            attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

            return attributeDescriptions;
        }
    };

    enum class pipelineLayoutTypes
    {
        Basic,
        Image,
        UboImage
    };

    enum class pipelineTypes
    {
        Graphic
    };

    enum class shaderModuleTypes
    {
        VertexSimple,
        FragmentSimple
    };

    enum class descriptorSetLayoutTypes
    {
        SamplerImage,
        Ubo,
        UboSampler
    };

    enum class descriptorSetTypes
    {
        Simple
    };

    class Device;
    class Swapchain;
    class Renderer
    {

        // const std::vector<Vertex> vertices = {
        //     {{-1.0f, -1.0f}, {1.0f, 0.3f, 0.7f}, {1.0f, 0.0f}},
        //     {{1.0f, -1.0f}, {0.7f, 1.0f, 0.3f}, {0.0f, 0.0f}},
        //     {{1.0f, 1.0f}, {0.7f, 0.3f, 1.0f}, {0.0f, 1.0f}},
        //     {{-1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}};

        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

        const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0};

    public:
        explicit Renderer(Device &);
        void initialize(SDL_Event *);
        void draw();
        void destroy();

    private:
        void recreateSwapChain();
        void createCommandBuffers();
        void createPipelineLayout(pipelineLayoutTypes, vk::DescriptorSetLayout *);
        void createPipeline(pipelineTypes, vk::PipelineLayout, vk::ShaderModule, vk::ShaderModule);
        void createShaderModule(shaderModuleTypes, std::string);
        void createDescriptorSetLayout(descriptorSetLayoutTypes, std::vector<vk::DescriptorSetLayoutBinding>);
        void createDescriptorPool();
        void createDescriptorSets(descriptorSetTypes, descriptorSetLayoutTypes, vk::ImageView, vk::Sampler);
        void recordCommandBuffer(vk::CommandBuffer, uint32_t);
        void updateUniformBuffer(uint32_t);
        void createUniformBuffers();

    private:
        Device &device;
        SDL_Event *event;
        Swapchain swapchain{device};
        Buffer buffer{device};
        Texture texture;
        std::vector<vk::CommandBuffer> commandBuffers;
        std::map<pipelineLayoutTypes, vk::PipelineLayout> pipelineLayouts;
        std::map<pipelineTypes, vk::Pipeline> pipelines;
        std::map<shaderModuleTypes, vk::ShaderModule> shaders;
        std::map<descriptorSetLayoutTypes, vk::DescriptorSetLayout> descriptorSetLayouts;
        vk::DescriptorPool descriptorPool;
        std::map<descriptorSetTypes, std::vector<vk::DescriptorSet>> descriptorSets;
        uint32_t currentFrame = 0;
        bool framebufferResized = false;

        vk::Buffer vertexBuffer;
        vk::DeviceMemory vertexBufferMemory;
        vk::Buffer indexBuffer;
        vk::DeviceMemory indexBufferMemory;

        std::vector<vk::Buffer> uniformBuffers;
        std::vector<vk::DeviceMemory> uniformBuffersMemory;
        std::vector<void *> uniformBuffersMapped;
    };

}
