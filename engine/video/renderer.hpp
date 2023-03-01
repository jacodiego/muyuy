#pragma once

#include "types.hpp"
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

#define TEXTURES_COUNT 4096

namespace muyuy::video
{

    class Device;
    class Swapchain;
    class Texture;
    class Renderer
    {
    public:
        explicit Renderer(Device &);
        void initialize(SDL_Event *);
        void startFrame();
        void endFrame();
        void draw(Texture *, int, int, int, int, int, int, float, float, float);
        void drawTiles(std::map<Texture *, std::vector<RenderTile>>);
        void resize() { framebufferResized = true; };
        void destroy();
        vk::DescriptorPool getDescriptorPool(poolTypes pt) { return descriptorPool.at(pt); };
        vk::DescriptorSetLayout getDescriptorSetLayout(descriptorTypes dt) { return descriptorSetLayouts.at(dt); };
        vk::Extent2D getWindowExtent() { return swapchain.swapChainExtent; };
        void resetScreenDescriptorPool();

    private:
        void recreateSwapChain();
        void createCommandBuffers();
        void createPipelineLayout(pipelineLayoutTypes, vk::DescriptorSetLayout *);
        void createPipeline(pipelineTypes, vk::PipelineLayout, vk::PipelineVertexInputStateCreateInfo, vk::ShaderModule, vk::ShaderModule);
        void createShaderModule(shaderModuleTypes, std::string);
        void createDescriptorSetLayout(descriptorTypes, std::vector<vk::DescriptorSetLayoutBinding>);
        void createDescriptorPool(poolTypes, std::vector<vk::DescriptorPoolSize>);
        void startRecordCommandBuffer();
        void endRecordCommandBuffer();

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
        std::map<poolTypes, vk::DescriptorPool> descriptorPool;
        uint32_t currentFrame = 0;
        bool framebufferResized = false;
        uint32_t currentImageIndex;

        // std::vector<BoundBuffer> vertexBuffers;
        // BoundBuffer indexBuffer;
    };

}
