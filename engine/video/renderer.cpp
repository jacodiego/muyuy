#include "renderer.hpp"

namespace muyuy::video
{

    Renderer::Renderer(Device &device)
        : device{device}
    {
    }

    void Renderer::initialize(SDL_Event *e)
    {
        event = e;
        swapchain.initialize();
        createCommandBuffers();

        // vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();
        // buffer.initialize(indices.data(), bufferSize);
        // buffer.createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer.buffer, indexBuffer.bufferMemory);
        // buffer.copyBuffer(indexBuffer.buffer, bufferSize);
        // buffer.destroy();

        vk::DescriptorSetLayoutBinding uboLayoutBinding{
            .binding = 0,
            .descriptorType = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = 1,
            .stageFlags = vk::ShaderStageFlagBits::eVertex,
            .pImmutableSamplers = nullptr};

        vk::DescriptorSetLayoutBinding samplerLayoutBinding{
            .binding = 1,
            .descriptorType = vk::DescriptorType::eCombinedImageSampler,
            .descriptorCount = 1,
            .stageFlags = vk::ShaderStageFlagBits::eFragment,
            .pImmutableSamplers = nullptr};

        std::vector<vk::DescriptorSetLayoutBinding> bindingsUboSampler = {uboLayoutBinding, samplerLayoutBinding};
        // samplerLayoutBinding.setBinding(0);
        // std::vector<vk::DescriptorSetLayoutBinding> bindingsSampler = {samplerLayoutBinding};

        createDescriptorSetLayout(descriptorTypes::Sampler, bindingsUboSampler);
        // createDescriptorSetLayout(descriptorTypes::UboSampler, bindingsUboSampler);

        // createPipelineLayout(pipelineLayoutTypes::Sampler, &descriptorSetLayouts.at(descriptorTypes::Sampler));
        createPipelineLayout(pipelineLayoutTypes::Sampler, &descriptorSetLayouts.at(descriptorTypes::Sampler));
        // createPipelineLayout(pipelineLayoutTypes::Sampler, nullptr);

        createShaderModule(shaderModuleTypes::VertexSampler, "engine/video/shaders/sampler_vert.spv");
        createShaderModule(shaderModuleTypes::FragmentSampler, "engine/video/shaders/sampler_frag.spv");
        // createShaderModule(shaderModuleTypes::VertexUboSampler, "engine/video/shaders/ubo_sampler_vert.spv");
        // createShaderModule(shaderModuleTypes::FragmentUboSampler, "engine/video/shaders/ubo_sampler_frag.spv");

        // -------------------------------------------------------
        // ----------- Create Pipeline
        // -------------------------------------------------------

        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
            .vertexBindingDescriptionCount = 1,
            .pVertexBindingDescriptions = &bindingDescription,
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
            .pVertexAttributeDescriptions = attributeDescriptions.data()};

        createPipeline(pipelineTypes::GraphicSampler, pipelineLayouts.at(pipelineLayoutTypes::Sampler), vertexInputInfo, shaders.at(shaderModuleTypes::VertexSampler), shaders.at(shaderModuleTypes::FragmentSampler));
        // createPipeline(pipelineTypes::GraphicUboSampler, pipelineLayouts.at(pipelineLayoutTypes::UboSampler), shaders.at(shaderModuleTypes::VertexUboSampler), shaders.at(shaderModuleTypes::FragmentUboSampler));

        // -------------------------------------------------------
        // ----------- Create Descriptors
        // -------------------------------------------------------

        vk::DescriptorPoolSize uniformBuffer{
            .type = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT) * 2};
        vk::DescriptorPoolSize combinedImageSampler{
            .type = vk::DescriptorType::eCombinedImageSampler,
            .descriptorCount = TEXTURES_COUNT};

        std::vector<vk::DescriptorPoolSize> poolSizes{combinedImageSampler, uniformBuffer};
        createDescriptorPool(descriptorTypes::Sampler, poolSizes);
        // poolSizes.push_back(uniformBuffer);
        // createDescriptorPool(descriptorTypes::UboSampler, poolSizes);

        //  createDescriptorSets(descriptorTypes::Sampler, texture.getImageView(), texture.getSampler());
        //  createDescriptorSets(descriptorTypes::UboSampler, texture.getImageView(), texture.getSampler());
    }

    void Renderer::destroy()
    {
        for (auto shader : shaders)
        {
            device.getDevice().destroyShaderModule(shader.second);
        }
        shaders.clear();
        for (auto pipeline : pipelines)
        {
            device.getDevice().destroyPipeline(pipeline.second);
        }
        pipelines.clear();
        for (auto pipelineLayout : pipelineLayouts)
        {
            device.getDevice().destroyPipelineLayout(pipelineLayout.second);
        }
        pipelineLayouts.clear();
    }

    void Renderer::startFrame()
    {

        device.getDevice().waitForFences(swapchain.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        vk::Result result;

        std::tie(result, currentImageIndex) = device.getDevice().acquireNextImageKHR(swapchain.swapChain, UINT64_MAX, swapchain.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE);

        if (result == vk::Result::eErrorOutOfDateKHR)
        {
            recreateSwapChain();
            return;
        }
        else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        device.getDevice().resetFences(swapchain.inFlightFences[currentFrame]);
        commandBuffers[currentFrame].reset();

        startRecordCommandBuffer();
    }

    void Renderer::endFrame()
    {
        endRecordCommandBuffer();
        vk::Semaphore waitSemaphores[] = {swapchain.imageAvailableSemaphores[currentFrame]};
        vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        vk::Semaphore signalSemaphores[] = {swapchain.renderFinishedSemaphores[currentFrame]};

        vk::SubmitInfo submitInfo{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = waitSemaphores,
            .pWaitDstStageMask = waitStages,
            .commandBufferCount = 1,
            .pCommandBuffers = &commandBuffers[currentFrame],
            .signalSemaphoreCount = 1,
            .pSignalSemaphores = signalSemaphores};

        device.getGraphicsQueue().submit(submitInfo, swapchain.inFlightFences[currentFrame]);

        vk::SwapchainKHR swapChains[] = {swapchain.swapChain};

        vk::PresentInfoKHR presentInfo{
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = signalSemaphores,
            .swapchainCount = 1,
            .pSwapchains = swapChains,
            .pImageIndices = &currentImageIndex};

        vk::Result result = device.getPresentQueue().presentKHR(&presentInfo);

        if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || framebufferResized)
        {
            framebufferResized = false;
            recreateSwapChain();
        }
        else if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        currentFrame = (currentFrame + 1) % Swapchain::MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::recreateSwapChain()
    {
        vk::Extent2D extent{
            .width = 0,
            .height = 0};
        while (extent.width == 0 || extent.height == 0)
        {
            extent = device.getWindowExtent();
            SDL_WaitEvent(event);
        }

        device.waitIdle();

        swapchain.cleanup();
        swapchain.createSwapChain();
        swapchain.createImageViews();
        swapchain.createFramebuffers();
    }

    void Renderer::createCommandBuffers()
    {
        commandBuffers.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);
        vk::CommandBufferAllocateInfo allocInfo{
            .commandPool = device.getCommandPool(),
            .level = vk::CommandBufferLevel::ePrimary,
            .commandBufferCount = (uint32_t)commandBuffers.size()};

        commandBuffers = device.getDevice().allocateCommandBuffers(allocInfo);
    }

    void Renderer::createPipelineLayout(pipelineLayoutTypes type, vk::DescriptorSetLayout *descriptor)
    {
        vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
        if (descriptor != nullptr)
        {
            pipelineLayoutInfo.setSetLayoutCount(1);
            pipelineLayoutInfo.setPSetLayouts(descriptor);
        }

        pipelineLayouts.insert(std::pair<pipelineLayoutTypes, vk::PipelineLayout>(type, device.getDevice().createPipelineLayout(pipelineLayoutInfo)));
    }

    void Renderer::createPipeline(pipelineTypes type, vk::PipelineLayout pipelineLayout, vk::PipelineVertexInputStateCreateInfo vertexInputInfo, vk::ShaderModule vertShader, vk::ShaderModule fragShader)
    {

        vk::PipelineShaderStageCreateInfo vertShaderStageInfo{
            .stage = vk::ShaderStageFlagBits::eVertex,
            .module = vertShader,
            .pName = "main"};

        vk::PipelineShaderStageCreateInfo fragShaderStageInfo{
            .stage = vk::ShaderStageFlagBits::eFragment,
            .module = fragShader,
            .pName = "main"};

        vk::PipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        std::vector<vk::DynamicState> dynamicStates = {
            vk::DynamicState::eViewport,
            vk::DynamicState::eScissor};

        vk::PipelineDynamicStateCreateInfo dynamicState{
            .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
            .pDynamicStates = dynamicStates.data()};

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly{
            .topology = vk::PrimitiveTopology::eTriangleList,
            .primitiveRestartEnable = VK_FALSE};

        vk::Viewport viewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = (float)swapchain.swapChainExtent.width,
            .height = (float)swapchain.swapChainExtent.height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f};

        vk::Rect2D scissor{
            .offset = {0, 0},
            .extent = swapchain.swapChainExtent};

        vk::PipelineViewportStateCreateInfo viewportState{
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor};

        vk::PipelineRasterizationStateCreateInfo rasterizer{
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = vk::PolygonMode::eFill,
            //.cullMode = vk::CullModeFlagBits::eBack,
            .cullMode = vk::CullModeFlagBits::eNone,
            .frontFace = vk::FrontFace::eCounterClockwise,
            //.frontFace = vk::FrontFace::eClockwise,
            .depthBiasEnable = VK_FALSE,
            .lineWidth = 1.0f};

        vk::PipelineMultisampleStateCreateInfo multisampling{
            .rasterizationSamples = vk::SampleCountFlagBits::e1,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 1.0f,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable = VK_FALSE};

        vk::PipelineColorBlendAttachmentState colorBlendAttachment{
            .blendEnable = VK_TRUE,
            .srcColorBlendFactor = vk::BlendFactor::eSrcAlpha,
            .dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha,
            .colorBlendOp = vk::BlendOp::eAdd,
            .srcAlphaBlendFactor = vk::BlendFactor::eOne,
            .dstAlphaBlendFactor = vk::BlendFactor::eZero,
            .alphaBlendOp = vk::BlendOp::eAdd,
            .colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};

        vk::PipelineColorBlendStateCreateInfo colorBlending{
            .logicOpEnable = VK_FALSE,
            .logicOp = vk::LogicOp::eCopy,
            .attachmentCount = 1,
            .pAttachments = &colorBlendAttachment,
            .blendConstants = {{1.0f, 1.0f, 1.0f, 1.0f}}};

        vk::GraphicsPipelineCreateInfo pipelineInfo{
            .stageCount = 2,
            .pStages = shaderStages,
            .pVertexInputState = &vertexInputInfo,
            .pInputAssemblyState = &inputAssembly,
            .pViewportState = &viewportState,
            .pRasterizationState = &rasterizer,
            .pMultisampleState = &multisampling,
            .pDepthStencilState = nullptr,
            .pColorBlendState = &colorBlending,
            .pDynamicState = &dynamicState,
            .layout = pipelineLayout,
            .renderPass = swapchain.renderPass,
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = -1};

        vk::Result result;
        vk::Pipeline pipeline;
        std::tie(result, pipeline) = device.getDevice().createGraphicsPipeline(VK_NULL_HANDLE, pipelineInfo);

        switch (result)
        {
        case vk::Result::eSuccess:
            pipelines.insert(std::pair<pipelineTypes, vk::Pipeline>(type, pipeline));
            break;
        case vk::Result::ePipelineCompileRequiredEXT:
            throw std::runtime_error("failed to create graphics pipeline!");
            break;
        default:
            assert(false);
        }
    }

    void Renderer::createShaderModule(shaderModuleTypes type, std::string pathfile)
    {
        auto shaderCode = utils::readFile(pathfile);

        vk::ShaderModuleCreateInfo createInfo{
            .codeSize = shaderCode.size(),
            .pCode = reinterpret_cast<const uint32_t *>(shaderCode.data())};

        shaders.insert(std::pair<shaderModuleTypes, vk::ShaderModule>(type, device.getDevice().createShaderModule(createInfo)));
    }

    void Renderer::createDescriptorSetLayout(descriptorTypes type, std::vector<vk::DescriptorSetLayoutBinding> bindings)
    {
        vk::DescriptorSetLayoutCreateInfo layoutInfo{
            .bindingCount = static_cast<uint32_t>(bindings.size()),
            .pBindings = bindings.data()};

        descriptorSetLayouts.insert(std::pair<descriptorTypes, vk::DescriptorSetLayout>(type, device.getDevice().createDescriptorSetLayout(layoutInfo)));
    }

    void Renderer::createDescriptorPool(descriptorTypes type, std::vector<vk::DescriptorPoolSize> poolSizes)
    {
        vk::DescriptorPoolCreateInfo poolInfo{
            .maxSets = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT) * 2 + TEXTURES_COUNT,
            .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
            .pPoolSizes = poolSizes.data()};

        descriptorPool.insert(std::pair<descriptorTypes, vk::DescriptorPool>(type, device.getDevice().createDescriptorPool(poolInfo)));
    }

    void Renderer::startRecordCommandBuffer()
    {
        vk::CommandBufferBeginInfo beginInfo{};

        if (commandBuffers[currentFrame].begin(&beginInfo) != vk::Result::eSuccess)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        vk::ClearValue clearColor{
            .color = {{{0.0f, 0.0f, 0.0f, 1.0f}}}};

        vk::RenderPassBeginInfo renderPassInfo{
            .renderPass = swapchain.renderPass,
            .framebuffer = swapchain.swapChainFramebuffers[currentImageIndex],
            .renderArea = {
                .offset = {0, 0},
                .extent = swapchain.swapChainExtent},
            .clearValueCount = 1,
            .pClearValues = &clearColor};

        commandBuffers[currentFrame].beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

        vk::Viewport viewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(swapchain.swapChainExtent.width),
            .height = static_cast<float>(swapchain.swapChainExtent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f};

        commandBuffers[currentFrame].setViewport(0, viewport);

        vk::Rect2D scissor{
            .offset = {0, 0},
            .extent = swapchain.swapChainExtent};

        commandBuffers[currentFrame].setScissor(0, scissor);

        commandBuffers[currentFrame].bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines.at(pipelineTypes::GraphicSampler));
    }

    void Renderer::endRecordCommandBuffer()
    {
        commandBuffers[currentFrame].endRenderPass();
        commandBuffers[currentFrame].end();
    }

    void Renderer::draw(Texture *texture, int x, int y, int offset_x, int offset_y, int width, int height, float alpha, float scale, float multiplyColor)
    {
        float halfWidth = swapchain.swapChainExtent.width / 2;
        float halfHeight = swapchain.swapChainExtent.height / 2;

        std::vector<Vertex> vertices = {
            {{(x + width) / halfWidth - 1.0f, (y + height) / halfHeight - 1.0f}, texture->getColor(), {(float)(offset_x + width) / (float)texture->getWidth(), (float)(offset_y + height) / (float)texture->getHeight()}},
            {{x / halfWidth - 1.0f, (y + height) / halfHeight - 1.0f}, texture->getColor(), {(float)offset_x / (float)texture->getWidth(), (float)(offset_y + height) / (float)texture->getHeight()}},
            {{x / halfWidth - 1.0f, y / halfHeight - 1.0f}, texture->getColor(), {(float)offset_x / (float)texture->getWidth(), (float)offset_y / (float)texture->getHeight()}},
            {{(x + width) / halfWidth - 1.0f, y / halfHeight - 1.0f}, texture->getColor(), {(float)(offset_x + width) / (float)texture->getWidth(), (float)offset_y / (float)texture->getHeight()}}};

        std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

        BoundBuffer tmpVertex;
        BoundBuffer tmpIndex;
        vk::DeviceSize bufferSize;

        bufferSize = sizeof(vertices[0]) * vertices.size();
        buffer.initialize(vertices.data(), bufferSize);
        buffer.createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, tmpVertex.buffer, tmpVertex.bufferMemory);
        buffer.copyBuffer(tmpVertex.buffer, bufferSize);
        buffer.destroy();

        bufferSize = sizeof(indices[0]) * indices.size();
        buffer.initialize(indices.data(), bufferSize);
        buffer.createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, tmpIndex.buffer, tmpIndex.bufferMemory);
        buffer.copyBuffer(tmpIndex.buffer, bufferSize);
        buffer.destroy();

        vk::Buffer vertexBuffers[] = {tmpVertex.buffer};
        vk::DeviceSize offsets[] = {0};

        commandBuffers[currentFrame].bindVertexBuffers(0, vertexBuffers, offsets);

        commandBuffers[currentFrame].bindIndexBuffer(tmpIndex.buffer, 0, vk::IndexType::eUint16);

        commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayouts.at(pipelineLayoutTypes::Sampler), 0, texture->getDescriptorSet(currentFrame), nullptr);
        commandBuffers[currentFrame].drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
        texture->updateUniformBuffer(currentFrame, alpha, scale, multiplyColor);
    }

    void Renderer::drawTiles(std::map<Texture *, std::vector<RenderTile>> texture_tiles)
    {
        float halfWidth = swapchain.swapChainExtent.width / 2;
        float halfHeight = swapchain.swapChainExtent.height / 2;
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;

        for (const auto &texture : texture_tiles)
        {
            int i = 0;
            for (auto tile : texture.second)
            {
                vertices.push_back({{(tile.x + tile.width) / halfWidth - 1.0f, (tile.y + tile.height) / halfHeight - 1.0f}, texture.first->getColor(), {(float)(tile.offset_x + tile.width) / (float)texture.first->getWidth(), (float)(tile.offset_y + tile.height) / (float)texture.first->getHeight()}});
                vertices.push_back({{tile.x / halfWidth - 1.0f, (tile.y + tile.height) / halfHeight - 1.0f}, texture.first->getColor(), {(float)tile.offset_x / (float)texture.first->getWidth(), (float)(tile.offset_y + tile.height) / (float)texture.first->getHeight()}});
                vertices.push_back({{tile.x / halfWidth - 1.0f, tile.y / halfHeight - 1.0f}, texture.first->getColor(), {(float)tile.offset_x / (float)texture.first->getWidth(), (float)tile.offset_y / (float)texture.first->getHeight()}});
                vertices.push_back({{(tile.x + tile.width) / halfWidth - 1.0f, tile.y / halfHeight - 1.0f}, texture.first->getColor(), {(float)(tile.offset_x + tile.width) / (float)texture.first->getWidth(), (float)tile.offset_y / (float)texture.first->getHeight()}});
                indices.push_back(i * 4);
                indices.push_back(i * 4 + 1);
                indices.push_back(i * 4 + 2);
                indices.push_back(i * 4 + 2);
                indices.push_back(i * 4 + 3);
                indices.push_back(i * 4);
                i++;
            }
            BoundBuffer tmpVertex;
            BoundBuffer tmpIndex;
            vk::DeviceSize bufferSize;

            bufferSize = sizeof(vertices[0]) * vertices.size();
            buffer.initialize(vertices.data(), bufferSize);
            buffer.createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, tmpVertex.buffer, tmpVertex.bufferMemory);
            buffer.copyBuffer(tmpVertex.buffer, bufferSize);
            buffer.destroy();

            bufferSize = sizeof(indices[0]) * indices.size();
            buffer.initialize(indices.data(), bufferSize);
            buffer.createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, tmpIndex.buffer, tmpIndex.bufferMemory);
            buffer.copyBuffer(tmpIndex.buffer, bufferSize);
            buffer.destroy();

            vk::Buffer vertexBuffers[] = {tmpVertex.buffer};
            vk::DeviceSize offsets[] = {0};

            commandBuffers[currentFrame].bindVertexBuffers(0, vertexBuffers, offsets);

            commandBuffers[currentFrame].bindIndexBuffer(tmpIndex.buffer, 0, vk::IndexType::eUint16);

            commandBuffers[currentFrame].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayouts.at(pipelineLayoutTypes::Sampler), 0, texture.first->getDescriptorSet(currentFrame), nullptr);
            commandBuffers[currentFrame].drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
            texture.first->updateUniformBuffer(currentFrame, 1, 1, 0);
        }
    }

}