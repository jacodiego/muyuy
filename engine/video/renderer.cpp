#include "renderer.hpp"

namespace muyuy::video
{

    Renderer::Renderer(Device &device)
        : device{device}, texture{device}
    {
    }

    void Renderer::initialize(SDL_Event *e)
    {
        event = e;
        swapchain.initialize();
        createCommandBuffers();

        texture.load("data/boot/logo.png");

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

        createUniformBuffers();

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

        std::vector<vk::DescriptorSetLayoutBinding> bindings = {uboLayoutBinding, samplerLayoutBinding};

        createDescriptorSetLayout(descriptorSetLayoutTypes::SamplerImage, bindings);

        createPipelineLayout(pipelineLayoutTypes::Image, &descriptorSetLayouts.at(descriptorSetLayoutTypes::SamplerImage));
        createShaderModule(shaderModuleTypes::VertexSimple, "engine/video/shaders/vert.spv");
        createShaderModule(shaderModuleTypes::FragmentSimple, "engine/video/shaders/frag.spv");
        createPipeline(pipelineTypes::Graphic, pipelineLayouts.at(pipelineLayoutTypes::Image), shaders.at(shaderModuleTypes::VertexSimple), shaders.at(shaderModuleTypes::FragmentSimple));
        createDescriptorPool();
        createDescriptorSets(descriptorSetTypes::Simple, descriptorSetLayoutTypes::SamplerImage, texture.getImageView(), texture.getSampler());
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
        updateUniformBuffer(currentFrame);

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
        swapchain.createRenderPass();
        createPipeline(pipelineTypes::Graphic, pipelineLayouts.at(pipelineLayoutTypes::Image), shaders.at(shaderModuleTypes::VertexSimple), shaders.at(shaderModuleTypes::FragmentSimple));
        swapchain.createFramebuffers();
        swapchain.createSwapChain();
        createCommandBuffers();
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
        // vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
        // if (descriptor != nullptr)
        // {
        vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
            .setLayoutCount = 1,
            .pSetLayouts = descriptor};
        //}

        pipelineLayouts.insert(std::pair<pipelineLayoutTypes, vk::PipelineLayout>(type, device.getDevice().createPipelineLayout(pipelineLayoutInfo)));
    }

    void Renderer::createPipeline(pipelineTypes type, vk::PipelineLayout pipelineLayout, vk::ShaderModule vertShader, vk::ShaderModule fragShader)
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

        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo{
            .vertexBindingDescriptionCount = 1,
            .pVertexBindingDescriptions = &bindingDescription,
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size()),
            .pVertexAttributeDescriptions = attributeDescriptions.data()};

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
            .frontFace = vk::FrontFace::eCounterClockwise,
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

    void Renderer::createDescriptorSetLayout(descriptorSetLayoutTypes type, std::vector<vk::DescriptorSetLayoutBinding> bindings)
    {
        vk::DescriptorSetLayoutCreateInfo layoutInfo{
            .bindingCount = static_cast<uint32_t>(bindings.size()),
            .pBindings = bindings.data()};

        descriptorSetLayouts.insert(std::pair<descriptorSetLayoutTypes, vk::DescriptorSetLayout>(type, device.getDevice().createDescriptorSetLayout(layoutInfo)));
    }

    void Renderer::createDescriptorPool()
    {
        vk::DescriptorPoolSize uniformBuffer{
            .type = vk::DescriptorType::eUniformBuffer,
            .descriptorCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT)};
        vk::DescriptorPoolSize combinedImageSampler{
            .type = vk::DescriptorType::eCombinedImageSampler,
            .descriptorCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT)};

        std::array<vk::DescriptorPoolSize, 2> poolSizes{uniformBuffer, combinedImageSampler};

        vk::DescriptorPoolCreateInfo poolInfo{
            .maxSets = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT),
            .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
            .pPoolSizes = poolSizes.data()};

        descriptorPool = device.getDevice().createDescriptorPool(poolInfo);
    }

    void Renderer::createDescriptorSets(descriptorSetTypes type, descriptorSetLayoutTypes layoutType, vk::ImageView imageView, vk::Sampler sampler)
    {
        std::vector<vk::DescriptorSetLayout> layouts(Swapchain::MAX_FRAMES_IN_FLIGHT, descriptorSetLayouts.at(layoutType));
        vk::DescriptorSetAllocateInfo allocInfo{
            .descriptorPool = descriptorPool,
            .descriptorSetCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT),
            .pSetLayouts = layouts.data()};

        descriptorSets.insert(std::pair<descriptorSetTypes, std::vector<vk::DescriptorSet>>(type, device.getDevice().allocateDescriptorSets(allocInfo)));

        for (size_t i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
        {
            vk::DescriptorBufferInfo bufferInfo{
                .buffer = uniformBuffers[i],
                .offset = 0,
                .range = sizeof(UniformBufferObject)};

            vk::DescriptorImageInfo imageInfo{
                .sampler = sampler,
                .imageView = imageView,
                .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal};

            vk::WriteDescriptorSet descUniformBuffers{
                .dstSet = descriptorSets.at(type)[i],
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eUniformBuffer,
                .pBufferInfo = &bufferInfo,
            };

            vk::WriteDescriptorSet descImageSampler{
                .dstSet = descriptorSets.at(type)[i],
                .dstBinding = 1,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                .pImageInfo = &imageInfo,
            };

            std::vector<vk::WriteDescriptorSet> descriptorWrites{descUniformBuffers, descImageSampler};

            device.getDevice().updateDescriptorSets(descriptorWrites, nullptr);
        }
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

        vk::Viewport viewport{
            .x = 0.0f,
            .y = 0.0f,
            .width = static_cast<float>(swapchain.swapChainExtent.width),
            .height = static_cast<float>(swapchain.swapChainExtent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f};

        commandBuffer.setViewport(0, viewport);

        vk::Rect2D scissor{
            .offset = {0, 0},
            .extent = swapchain.swapChainExtent};

        commandBuffer.setScissor(0, scissor);

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelines.at(pipelineTypes::Graphic));

        vk::Buffer vertexBuffers[] = {vertexBuffer};
        vk::DeviceSize offsets[] = {0};

        commandBuffer.bindVertexBuffers(0, vertexBuffers, offsets);

        commandBuffer.bindIndexBuffer(indexBuffer, 0, vk::IndexType::eUint16);

        commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipelineLayouts.at(pipelineLayoutTypes::Image), 0, descriptorSets.at(descriptorSetTypes::Simple)[currentFrame], nullptr);

        commandBuffer.drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

        commandBuffer.endRenderPass();
        commandBuffer.end();
    }

    void Renderer::updateUniformBuffer(uint32_t currentImage)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{
            .model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            .view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            .proj = glm::perspective(glm::radians(45.0f), swapchain.swapChainExtent.width / (float)swapchain.swapChainExtent.height, 0.1f, 10.0f)};

        ubo.proj[1][1] *= -1;

        memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

    void Renderer::createUniformBuffers()
    {
        vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
        {
            buffer.createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, uniformBuffers[i], uniformBuffersMemory[i]);
            uniformBuffersMapped[i] = device.getDevice().mapMemory(uniformBuffersMemory[i], 0, bufferSize);
        }
    }
}