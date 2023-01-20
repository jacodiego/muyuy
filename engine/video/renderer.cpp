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
        swapchain = std::make_unique<Swapchain>(device);
        recreateSwapChain();
        createCommandBuffers();

        createPipelineLayout(pipelineLayoutTypes::Basic, nullptr);
        createShaderModule(shaderModuleTypes::VertexSimple, "engine/video/shaders/vert.spv");
        createShaderModule(shaderModuleTypes::FragmentSimple, "engine/video/shaders/frag.spv");
        createPipeline(pipelineTypes::Graphic, pipelineLayouts.at(pipelineLayoutTypes::Basic), shaders.at(shaderModuleTypes::VertexSimple), shaders.at(shaderModuleTypes::FragmentSimple));
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

        // cleanupSwapChain();

        // createSwapChain();
        // createImageViews();
        // createRenderPass();
        // createGraphicsPipeline();
        // createFramebuffers();
        // createCommandBuffers();
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
            vk::PipelineLayoutCreateInfo pipelineLayoutInfo{
                .setLayoutCount = 1,
                .pSetLayouts = descriptor};
        }

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
            .width = (float)swapchain->getSwapChainExtent().width,
            .height = (float)swapchain->getSwapChainExtent().height,
            .minDepth = 0.0f,
            .maxDepth = 1.0f};

        vk::Rect2D scissor{
            .offset = {0, 0},
            .extent = swapchain->getSwapChainExtent()};

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
            .renderPass = swapchain->getRenderPass(),
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
}