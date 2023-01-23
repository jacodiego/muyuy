#pragma once

#include "device.hpp"
#include "swapchain.hpp"
#include "texture.hpp"

#include "utils/file.hpp"

#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <map>

namespace muyuy::video
{

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
        // glm::vec2 texCoord;

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
            attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            // attributeDescriptions[2].binding = 0;
            // attributeDescriptions[2].location = 2;
            // attributeDescriptions[2].format = vk::Format::eR32G32Sfloat;
            // attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

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
    public:
        explicit Renderer(Device &);
        void initialize(SDL_Event *);
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

    private:
        Device &device;
        SDL_Event *event;
        std::unique_ptr<Swapchain> swapchain;
        Texture texture;
        std::vector<vk::CommandBuffer> commandBuffers;
        std::map<pipelineLayoutTypes, vk::PipelineLayout> pipelineLayouts;
        std::map<pipelineTypes, vk::Pipeline> pipelines;
        std::map<shaderModuleTypes, vk::ShaderModule> shaders;
        std::map<descriptorSetLayoutTypes, vk::DescriptorSetLayout> descriptorSetLayouts;
        vk::DescriptorPool descriptorPool;
        std::map<descriptorSetTypes, std::vector<vk::DescriptorSet>> descriptorSets;
    };

}
