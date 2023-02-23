#pragma once

#include "texture.hpp"

namespace muyuy::video
{
    class Texture;
    struct RenderTile
    {
        Texture *texture;
        int x;
        int y;
        int offset_x;
        int offset_y;
        int width;
        int height;
    };
    struct BoundBuffer
    {
        vk::Buffer buffer;
        vk::DeviceMemory bufferMemory;
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

}