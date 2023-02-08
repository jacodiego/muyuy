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

        vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
        buffer.initialize(vertices.data(), bufferSize);
        buffer.createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer, vertexBufferMemory);
        buffer.copyBuffer(vertexBuffer, bufferSize);
        buffer.destroy();

        bufferSize = sizeof(indices[0]) * indices.size();

        buffer.initialize(indices.data(), bufferSize);
        buffer.createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, indexBuffer, indexBufferMemory);
        buffer.copyBuffer(indexBuffer, bufferSize);
        buffer.destroy();

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
            .descriptorCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT) * TEXTURES_COUNT};
        vk::DescriptorPoolSize combinedImageSampler{
            .type = vk::DescriptorType::eCombinedImageSampler,
            .descriptorCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT) * TEXTURES_COUNT};

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

    void Renderer::draw()
    {

        device.getDevice().waitForFences(swapchain.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vk::Result result;

        std::tie(result, imageIndex) = device.getDevice().acquireNextImageKHR(swapchain.swapChain, UINT64_MAX, swapchain.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE);

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

        recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

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
            .pImageIndices = &imageIndex};

        result = device.getPresentQueue().presentKHR(&presentInfo);

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
            .cullMode = vk::CullModeFlagBits::eBack,
            //.frontFace = vk::FrontFace::eCounterClockwise,
            .frontFace = vk::FrontFace::eClockwise,
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
            .blendConstants = {{0.0f, 0.0f, 0.0f, 0.0f}}};

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
            .maxSets = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT),
            .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
            .pPoolSizes = poolSizes.data()};

        descriptorPool.insert(std::pair<descriptorTypes, vk::DescriptorPool>(type, device.getDevice().createDescriptorPool(poolInfo)));
    }

    void Renderer::recordCommandBuffer(vk::CommandBuffer commandBuffer, uint32_t imageIndex)
    {
        vk::CommandBufferBeginInfo beginInfo{};

        if (commandBuffer.begin(&beginInfo) != vk::Result::eSuccess)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        vk::ClearValue clearColor{
            .color = {{{1.0f, 1.0f, 1.0f, 1.0f}}}};

        vk::RenderPassBeginInfo renderPassInfo{
            .renderPass = swapchain.renderPass,
            .framebuffer = swapchain.swapChainFramebuffers[imageIndex],
            .renderArea = {
                .offset = {0, 0},
                .extent = swapchain.swapChainExtent},
            .clearValueCount = 1,
            .pClearValues = &clearColor};

        commandBuffer.beginRenderPass(&renderPassInfo, vk::SubpassContents::eInline);

        vk::Rect2D scissor{
            .offset = {0, 0},
            .extent = swapchain.swapChainExtent};

        commandBuffer.setScissor(0, scissor);

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines.at(pipelineTypes::GraphicSampler));

        vk::Buffer vertexBuffers[] = {vertexBuffer};
        vk::DeviceSize offsets[] = {0};

        commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);

        commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint16);

        for (auto texture : _draw_textures)
        {
            commandBuffer.setViewport(0, texture->getViewport());
            commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayouts.at(pipelineLayoutTypes::Sampler), 0, texture->getDescriptorSet(currentFrame), nullptr);
            commandBuffer.drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
            texture->updateUniformBuffer(currentFrame);
        }
        commandBuffer.endRenderPass();
        commandBuffer.end();
    }

    void Renderer::addDrawTexture(Texture *texture)
    {
        _draw_textures.push_back(texture);
    }
    void Renderer::removeDrawTexture(Texture *texture)
    {
        _draw_textures.erase(std::remove(_draw_textures.begin(), _draw_textures.end(), texture), _draw_textures.end());
    }
}