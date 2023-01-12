// #include "video_hpp.hpp"

// namespace muyuy
// {
//     namespace video
//     {

//         VideoEngine *videoManager = nullptr;

//         bool VideoEngine::singletonInitialize()
//         {
//             return true;
//         }

//         void VideoEngine::initialize(SDL_Window *win, SDL_Event *ev)
//         {
//             window = win;
//             event = ev;
//             createInstance();
//             setupDebugCallback();
//             createSurface();
//             pickPhysicalDevice();
//             createLogicalDevice();
//             createSwapChain();
//             createImageViews();
//             createRenderPass();
//             createGraphicsPipeline();
//             createFramebuffers();
//             createCommandPool();
//             createVertexBuffer();
//             createCommandBuffers();
//             createSyncObjects();
//         }

//         void VideoEngine::drawFrame()
//         {
//             device->waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

//             uint32_t imageIndex;
//             try
//             {
//                 vk::ResultValue result = device->acquireNextImageKHR(swapChain, std::numeric_limits<uint64_t>::max(),
//                                                                      imageAvailableSemaphores[currentFrame], nullptr);
//                 imageIndex = result.value;
//             }
//             catch (vk::OutOfDateKHRError err)
//             {
//                 recreateSwapChain();
//                 return;
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to acquire swap chain image!");
//             }

//             vk::SubmitInfo submitInfo = {};

//             vk::Semaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
//             vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
//             submitInfo.waitSemaphoreCount = 1;
//             submitInfo.pWaitSemaphores = waitSemaphores;
//             submitInfo.pWaitDstStageMask = waitStages;

//             submitInfo.commandBufferCount = 1;
//             submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

//             vk::Semaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
//             submitInfo.signalSemaphoreCount = 1;
//             submitInfo.pSignalSemaphores = signalSemaphores;

//             device->resetFences(1, &inFlightFences[currentFrame]);

//             try
//             {
//                 graphicsQueue.submit(submitInfo, inFlightFences[currentFrame]);
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to submit draw command buffer!");
//             }

//             vk::PresentInfoKHR presentInfo = {};
//             presentInfo.waitSemaphoreCount = 1;
//             presentInfo.pWaitSemaphores = signalSemaphores;

//             vk::SwapchainKHR swapChains[] = {swapChain};
//             presentInfo.swapchainCount = 1;
//             presentInfo.pSwapchains = swapChains;
//             presentInfo.pImageIndices = &imageIndex;

//             vk::Result resultPresent;
//             try
//             {
//                 resultPresent = presentQueue.presentKHR(presentInfo);
//             }
//             catch (vk::OutOfDateKHRError err)
//             {
//                 resultPresent = vk::Result::eErrorOutOfDateKHR;
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to present swap chain image!");
//             }

//             if (resultPresent == vk::Result::eSuboptimalKHR || resultPresent == vk::Result::eSuboptimalKHR || framebufferResized)
//             {
//                 framebufferResized = false;
//                 recreateSwapChain();
//                 return;
//             }

//             currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
//         }

//         void VideoEngine::waitIdle()
//         {
//             device->waitIdle();
//         }

//         void VideoEngine::cleanup()
//         {
//             cleanupSwapChain();

//             device->destroyBuffer(vertexBuffer);
//             device->freeMemory(vertexBufferMemory);

//             for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//             {
//                 device->destroySemaphore(renderFinishedSemaphores[i]);
//                 device->destroySemaphore(imageAvailableSemaphores[i]);
//                 device->destroyFence(inFlightFences[i]);
//             }

//             device->destroyCommandPool(commandPool);

//             // surface is created by glfw, therefore not using a Unique handle
//             instance->destroySurfaceKHR(surface);

//             if (enableValidationLayers)
//             {
//                 DestroyDebugUtilsMessengerEXT(*instance, callback, nullptr);
//             }
//         }

//         void VideoEngine::cleanupSwapChain()
//         {
//             for (auto framebuffer : swapChainFramebuffers)
//             {
//                 device->destroyFramebuffer(framebuffer);
//             }

//             device->freeCommandBuffers(commandPool, commandBuffers);

//             device->destroyPipeline(graphicsPipeline);
//             device->destroyPipelineLayout(pipelineLayout);
//             device->destroyRenderPass(renderPass);

//             for (auto imageView : swapChainImageViews)
//             {
//                 device->destroyImageView(imageView);
//             }

//             device->destroySwapchainKHR(swapChain);
//         }

//         void VideoEngine::createInstance()
//         {
//             if (enableValidationLayers && !checkValidationLayerSupport())
//             {
//                 throw std::runtime_error("validation layers requested, but not available!");
//             }

//             auto appInfo = vk::ApplicationInfo(
//                 "Hello Triangle",
//                 VK_MAKE_VERSION(1, 0, 0),
//                 "No Engine",
//                 VK_MAKE_VERSION(1, 0, 0),
//                 VK_API_VERSION_1_0);

//             auto extensions = getRequiredExtensions();

//             auto createInfo = vk::InstanceCreateInfo(
//                 vk::InstanceCreateFlags(),
//                 &appInfo,
//                 0, nullptr,                                                 // enabled layers
//                 static_cast<uint32_t>(extensions.size()), extensions.data() // enabled extensions
//             );

//             if (enableValidationLayers)
//             {
//                 createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//                 createInfo.ppEnabledLayerNames = validationLayers.data();
//             }

//             try
//             {
//                 instance = vk::createInstanceUnique(createInfo, nullptr);
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to create instance!");
//             }
//         }

//         bool VideoEngine::checkValidationLayerSupport()
//         {
//             auto availableLayers = vk::enumerateInstanceLayerProperties();
//             for (const char *layerName : validationLayers)
//             {
//                 bool layerFound = false;

//                 for (const auto &layerProperties : availableLayers)
//                 {
//                     if (strcmp(layerName, layerProperties.layerName) == 0)
//                     {
//                         layerFound = true;
//                         break;
//                     }
//                 }

//                 if (!layerFound)
//                 {
//                     return false;
//                 }
//             }

//             return true;
//         }

//         std::vector<const char *> VideoEngine::getRequiredExtensions()
//         {
//             unsigned int sdlExtensionCount = 0;
//             SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, nullptr);
//             std::vector<const char *> extensions(sdlExtensionCount);
//             SDL_Vulkan_GetInstanceExtensions(window, &sdlExtensionCount, extensions.data());

//             if (enableValidationLayers)
//             {
//                 extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//             }

//             return extensions;
//         }

//         void VideoEngine::setupDebugCallback()
//         {
//             if (!enableValidationLayers)
//                 return;

//             auto createInfo = vk::DebugUtilsMessengerCreateInfoEXT(
//                 vk::DebugUtilsMessengerCreateFlagsEXT(),
//                 vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
//                 vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
//                 debugCallback,
//                 nullptr);

//             // NOTE: Vulkan-hpp has methods for this, but they trigger linking errors...
//             // instance->createDebugUtilsMessengerEXT(createInfo);
//             // instance->createDebugUtilsMessengerEXTUnique(createInfo);

//             // NOTE: reinterpret_cast is also used by vulkan.hpp internally for all these structs
//             if (CreateDebugUtilsMessengerEXT(*instance, reinterpret_cast<const VkDebugUtilsMessengerCreateInfoEXT *>(&createInfo), nullptr, &callback) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to set up debug callback!");
//             }
//         }

//         void VideoEngine::createSurface()
//         {
//             VkSurfaceKHR rawSurface;
//             if (SDL_Vulkan_CreateSurface(window, *instance, &rawSurface) == 0)
//             {
//                 throw std::runtime_error("failed to create window surface!");
//             }

//             surface = rawSurface;
//         }

//         void VideoEngine::pickPhysicalDevice()
//         {
//             auto devices = instance->enumeratePhysicalDevices();
//             if (devices.size() == 0)
//             {
//                 throw std::runtime_error("failed to find GPUs with Vulkan support!");
//             }

//             for (const auto &device : devices)
//             {
//                 if (isDeviceSuitable(device))
//                 {
//                     physicalDevice = device;
//                     break;
//                 }
//             }

//             if (!physicalDevice)
//             {
//                 throw std::runtime_error("failed to find a suitable GPU!");
//             }
//         }

//         void VideoEngine::createLogicalDevice()
//         {
//             QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

//             std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
//             std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

//             float queuePriority = 1.0f;

//             for (uint32_t queueFamily : uniqueQueueFamilies)
//             {
//                 queueCreateInfos.push_back({vk::DeviceQueueCreateFlags(),
//                                             queueFamily,
//                                             1, // queueCount
//                                             &queuePriority});
//             }

//             auto deviceFeatures = vk::PhysicalDeviceFeatures();
//             auto createInfo = vk::DeviceCreateInfo(
//                 vk::DeviceCreateFlags(),
//                 static_cast<uint32_t>(queueCreateInfos.size()),
//                 queueCreateInfos.data());
//             createInfo.pEnabledFeatures = &deviceFeatures;
//             createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
//             createInfo.ppEnabledExtensionNames = deviceExtensions.data();

//             if (enableValidationLayers)
//             {
//                 createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
//                 createInfo.ppEnabledLayerNames = validationLayers.data();
//             }

//             try
//             {
//                 device = physicalDevice.createDeviceUnique(createInfo);
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to create logical device!");
//             }

//             graphicsQueue = device->getQueue(indices.graphicsFamily.value(), 0);
//             presentQueue = device->getQueue(indices.presentFamily.value(), 0);
//         }

//         bool VideoEngine::isDeviceSuitable(const vk::PhysicalDevice &device)
//         {
//             QueueFamilyIndices indices = findQueueFamilies(device);

//             bool extensionsSupported = checkDeviceExtensionSupport(device);

//             bool swapChainAdequate = false;
//             if (extensionsSupported)
//             {
//                 SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
//                 swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
//             }

//             return indices.isComplete() && extensionsSupported && swapChainAdequate;
//         }

//         QueueFamilyIndices VideoEngine::findQueueFamilies(vk::PhysicalDevice device)
//         {
//             QueueFamilyIndices indices;

//             auto queueFamilies = device.getQueueFamilyProperties();

//             int i = 0;
//             for (const auto &queueFamily : queueFamilies)
//             {
//                 if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
//                 {
//                     indices.graphicsFamily = i;
//                 }

//                 if (queueFamily.queueCount > 0 && device.getSurfaceSupportKHR(i, surface))
//                 {
//                     indices.presentFamily = i;
//                 }

//                 if (indices.isComplete())
//                 {
//                     break;
//                 }

//                 i++;
//             }

//             return indices;
//         }

//         bool VideoEngine::checkDeviceExtensionSupport(const vk::PhysicalDevice &device)
//         {
//             std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

//             for (const auto &extension : device.enumerateDeviceExtensionProperties())
//             {
//                 requiredExtensions.erase(extension.extensionName);
//             }

//             return requiredExtensions.empty();
//         }

//         SwapChainSupportDetails VideoEngine::querySwapChainSupport(const vk::PhysicalDevice &device)
//         {
//             SwapChainSupportDetails details;
//             details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
//             details.formats = device.getSurfaceFormatsKHR(surface);
//             details.presentModes = device.getSurfacePresentModesKHR(surface);

//             return details;
//         }

//         void VideoEngine::createSwapChain()
//         {
//             SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

//             vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
//             vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
//             vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

//             uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
//             if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
//             {
//                 imageCount = swapChainSupport.capabilities.maxImageCount;
//             }

//             vk::SwapchainCreateInfoKHR createInfo(
//                 vk::SwapchainCreateFlagsKHR(),
//                 surface,
//                 imageCount,
//                 surfaceFormat.format,
//                 surfaceFormat.colorSpace,
//                 extent,
//                 1, // imageArrayLayers
//                 vk::ImageUsageFlagBits::eColorAttachment);

//             QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
//             uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

//             if (indices.graphicsFamily != indices.presentFamily)
//             {
//                 createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
//                 createInfo.queueFamilyIndexCount = 2;
//                 createInfo.pQueueFamilyIndices = queueFamilyIndices;
//             }
//             else
//             {
//                 createInfo.imageSharingMode = vk::SharingMode::eExclusive;
//             }

//             createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
//             createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
//             createInfo.presentMode = presentMode;
//             createInfo.clipped = VK_TRUE;

//             createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

//             try
//             {
//                 swapChain = device->createSwapchainKHR(createInfo);
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to create swap chain!");
//             }

//             swapChainImages = device->getSwapchainImagesKHR(swapChain);

//             swapChainImageFormat = surfaceFormat.format;
//             swapChainExtent = extent;
//         }

//         vk::SurfaceFormatKHR VideoEngine::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats)
//         {
//             if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
//             {
//                 return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
//             }

//             for (const auto &availableFormat : availableFormats)
//             {
//                 if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
//                 {
//                     return availableFormat;
//                 }
//             }

//             return availableFormats[0];
//         }

//         vk::PresentModeKHR VideoEngine::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes)
//         {
//             vk::PresentModeKHR bestMode = vk::PresentModeKHR::eFifo;

//             for (const auto &availablePresentMode : availablePresentModes)
//             {
//                 if (availablePresentMode == vk::PresentModeKHR::eMailbox)
//                 {
//                     return availablePresentMode;
//                 }
//                 else if (availablePresentMode == vk::PresentModeKHR::eImmediate)
//                 {
//                     bestMode = availablePresentMode;
//                 }
//             }

//             return bestMode;
//         }

//         vk::Extent2D VideoEngine::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities)
//         {
//             if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
//             {
//                 return capabilities.currentExtent;
//             }
//             else
//             {
//                 int width, height;
//                 SDL_Vulkan_GetDrawableSize(window, &width, &height);

//                 vk::Extent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

//                 actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
//                 actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

//                 return actualExtent;
//             }
//         }

//         void VideoEngine::createImageViews()
//         {
//             swapChainImageViews.resize(swapChainImages.size());

//             for (size_t i = 0; i < swapChainImages.size(); i++)
//             {
//                 vk::ImageViewCreateInfo createInfo = {};
//                 createInfo.image = swapChainImages[i];
//                 createInfo.viewType = vk::ImageViewType::e2D;
//                 createInfo.format = swapChainImageFormat;
//                 createInfo.components.r = vk::ComponentSwizzle::eIdentity;
//                 createInfo.components.g = vk::ComponentSwizzle::eIdentity;
//                 createInfo.components.b = vk::ComponentSwizzle::eIdentity;
//                 createInfo.components.a = vk::ComponentSwizzle::eIdentity;
//                 createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
//                 createInfo.subresourceRange.baseMipLevel = 0;
//                 createInfo.subresourceRange.levelCount = 1;
//                 createInfo.subresourceRange.baseArrayLayer = 0;
//                 createInfo.subresourceRange.layerCount = 1;

//                 try
//                 {
//                     swapChainImageViews[i] = device->createImageView(createInfo);
//                 }
//                 catch (vk::SystemError err)
//                 {
//                     throw std::runtime_error("failed to create image views!");
//                 }
//             }
//         }

//         void VideoEngine::createRenderPass()
//         {
//             vk::AttachmentDescription colorAttachment = {};
//             colorAttachment.format = swapChainImageFormat;
//             colorAttachment.samples = vk::SampleCountFlagBits::e1;
//             colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
//             colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
//             colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
//             colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
//             colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
//             colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

//             vk::AttachmentReference colorAttachmentRef = {};
//             colorAttachmentRef.attachment = 0;
//             colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

//             vk::SubpassDescription subpass = {};
//             subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
//             subpass.colorAttachmentCount = 1;
//             subpass.pColorAttachments = &colorAttachmentRef;

//             vk::SubpassDependency dependency = {};
//             dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
//             dependency.dstSubpass = 0;
//             dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
//             // dependency.srcAccessMask = 0;
//             dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
//             dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

//             vk::RenderPassCreateInfo renderPassInfo = {};
//             renderPassInfo.attachmentCount = 1;
//             renderPassInfo.pAttachments = &colorAttachment;
//             renderPassInfo.subpassCount = 1;
//             renderPassInfo.pSubpasses = &subpass;
//             renderPassInfo.dependencyCount = 1;
//             renderPassInfo.pDependencies = &dependency;

//             try
//             {
//                 renderPass = device->createRenderPass(renderPassInfo);
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to create render pass!");
//             }
//         }

//         void VideoEngine::createGraphicsPipeline()
//         {
//             auto vertShaderCode = readFile("shaders/vert.spv");
//             auto fragShaderCode = readFile("shaders/frag.spv");

//             auto vertShaderModule = createShaderModule(vertShaderCode);
//             auto fragShaderModule = createShaderModule(fragShaderCode);

//             std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {
//                 vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(),
//                                                   vk::ShaderStageFlagBits::eVertex,
//                                                   *vertShaderModule,
//                                                   "main"),
//                 vk::PipelineShaderStageCreateInfo(vk::PipelineShaderStageCreateFlags(),
//                                                   vk::ShaderStageFlagBits::eFragment,
//                                                   *fragShaderModule,
//                                                   "main")};

//             auto bindingDescription = Vertex::getBindingDescription();
//             auto attributeDescriptions = Vertex::getAttributeDescriptions();

//             vk::PipelineVertexInputStateCreateInfo vertexInputInfo(vk::PipelineVertexInputStateCreateFlags(),
//                                                                    bindingDescription,
//                                                                    attributeDescriptions);

//             vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
//             inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;
//             inputAssembly.primitiveRestartEnable = VK_FALSE;

//             vk::Viewport viewport = {};
//             viewport.x = 0.0f;
//             viewport.y = 0.0f;
//             viewport.width = (float)swapChainExtent.width;
//             viewport.height = (float)swapChainExtent.height;
//             viewport.minDepth = 0.0f;
//             viewport.maxDepth = 1.0f;

//             vk::Rect2D scissor = {};
//             scissor.offset = vk::Offset2D{0, 0};
//             scissor.extent = swapChainExtent;

//             vk::PipelineViewportStateCreateInfo viewportState = {};
//             viewportState.viewportCount = 1;
//             viewportState.pViewports = &viewport;
//             viewportState.scissorCount = 1;
//             viewportState.pScissors = &scissor;

//             vk::PipelineRasterizationStateCreateInfo rasterizer = {};
//             rasterizer.depthClampEnable = VK_FALSE;
//             rasterizer.rasterizerDiscardEnable = VK_FALSE;
//             rasterizer.polygonMode = vk::PolygonMode::eFill;
//             rasterizer.lineWidth = 1.0f;
//             rasterizer.cullMode = vk::CullModeFlagBits::eBack;
//             rasterizer.frontFace = vk::FrontFace::eClockwise;
//             rasterizer.depthBiasEnable = VK_FALSE;

//             vk::PipelineMultisampleStateCreateInfo multisampling = {};
//             multisampling.sampleShadingEnable = VK_FALSE;
//             multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

//             vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
//             colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
//             colorBlendAttachment.blendEnable = VK_FALSE;

//             vk::PipelineColorBlendStateCreateInfo colorBlending = {};
//             colorBlending.logicOpEnable = VK_FALSE;
//             colorBlending.logicOp = vk::LogicOp::eCopy;
//             colorBlending.attachmentCount = 1;
//             colorBlending.pAttachments = &colorBlendAttachment;
//             colorBlending.blendConstants[0] = 0.0f;
//             colorBlending.blendConstants[1] = 0.0f;
//             colorBlending.blendConstants[2] = 0.0f;
//             colorBlending.blendConstants[3] = 0.0f;

//             createDescriptorSetLayout();

//             vk::PipelineLayout pipelineLayout = device->createPipelineLayout(
//                 vk::PipelineLayoutCreateInfo(vk::PipelineLayoutCreateFlags(), *descriptorSetLayout));

//             vk::GraphicsPipelineCreateInfo pipelineInfo(vk::PipelineCreateFlags(),
//                                                         shaderStages,
//                                                         &vertexInputInfo,
//                                                         &inputAssembly,
//                                                         nullptr,
//                                                         &viewportState,
//                                                         &rasterizer,
//                                                         &multisampling,
//                                                         nullptr,
//                                                         &colorBlending,
//                                                         nullptr,
//                                                         pipelineLayout,
//                                                         renderPass);
//             try
//             {
//                 graphicsPipeline = device->createGraphicsPipeline(nullptr, pipelineInfo).value;
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to create graphics pipeline!");
//             }
//         }

//         vk::UniqueShaderModule VideoEngine::createShaderModule(const std::vector<char> &code)
//         {
//             try
//             {
//                 return device->createShaderModuleUnique({vk::ShaderModuleCreateFlags(),
//                                                          code.size(),
//                                                          reinterpret_cast<const uint32_t *>(code.data())});
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to create shader module!");
//             }
//         }

//         void VideoEngine::createDescriptorSetLayout()
//         {
//             vk::DescriptorSetLayoutBinding uboLayoutBinding(0,
//                                                             vk::DescriptorType::eUniformBuffer,
//                                                             vk::ShaderStageFlagBits::eVertex,
//                                                             nullptr);

//             vk::DescriptorSetLayoutBinding samplerLayoutBinding(1,
//                                                                 vk::DescriptorType::eCombinedImageSampler,
//                                                                 vk::ShaderStageFlagBits::eFragment,
//                                                                 nullptr);

//             std::array<vk::DescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
//             vk::DescriptorSetLayoutCreateInfo layoutInfo(vk::DescriptorSetLayoutCreateFlags(), bindings);

//             descriptorSetLayout = device->createDescriptorSetLayoutUnique(layoutInfo);
//         }

//         void VideoEngine::createFramebuffers()
//         {
//             swapChainFramebuffers.resize(swapChainImageViews.size());

//             for (size_t i = 0; i < swapChainImageViews.size(); i++)
//             {
//                 vk::ImageView attachments[] = {
//                     swapChainImageViews[i]};

//                 vk::FramebufferCreateInfo framebufferInfo = {};
//                 framebufferInfo.renderPass = renderPass;
//                 framebufferInfo.attachmentCount = 1;
//                 framebufferInfo.pAttachments = attachments;
//                 framebufferInfo.width = swapChainExtent.width;
//                 framebufferInfo.height = swapChainExtent.height;
//                 framebufferInfo.layers = 1;

//                 try
//                 {
//                     swapChainFramebuffers[i] = device->createFramebuffer(framebufferInfo);
//                 }
//                 catch (vk::SystemError err)
//                 {
//                     throw std::runtime_error("failed to create framebuffer!");
//                 }
//             }
//         }

//         void VideoEngine::createCommandPool()
//         {
//             QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

//             vk::CommandPoolCreateInfo poolInfo = {};
//             poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

//             try
//             {
//                 commandPool = device->createCommandPool(poolInfo);
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to create command pool!");
//             }
//         }

//         void VideoEngine::createVertexBuffer()
//         {
//             vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

//             vk::Buffer stagingBuffer;
//             vk::DeviceMemory stagingBufferMemory;
//             createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

//             void *data = device->mapMemory(stagingBufferMemory, 0, bufferSize);
//             memcpy(data, vertices.data(), (size_t)bufferSize);
//             device->unmapMemory(stagingBufferMemory);

//             createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer, vk::MemoryPropertyFlagBits::eDeviceLocal, vertexBuffer, vertexBufferMemory);

//             copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

//             device->destroyBuffer(stagingBuffer);
//             device->freeMemory(stagingBufferMemory);
//         }

//         void VideoEngine::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &bufferMemory)
//         {
//             vk::BufferCreateInfo bufferInfo = {};
//             bufferInfo.size = size;
//             bufferInfo.usage = usage;
//             bufferInfo.sharingMode = vk::SharingMode::eExclusive;

//             try
//             {
//                 buffer = device->createBuffer(bufferInfo);
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to create buffer!");
//             }

//             vk::MemoryRequirements memRequirements = device->getBufferMemoryRequirements(buffer);

//             vk::MemoryAllocateInfo allocInfo = {};
//             allocInfo.allocationSize = memRequirements.size;
//             allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

//             try
//             {
//                 bufferMemory = device->allocateMemory(allocInfo);
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to allocate buffer memory!");
//             }

//             device->bindBufferMemory(buffer, bufferMemory, 0);
//         }

//         void VideoEngine::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
//         {
//             vk::CommandBufferAllocateInfo allocInfo = {};
//             allocInfo.level = vk::CommandBufferLevel::ePrimary;
//             allocInfo.commandPool = commandPool;
//             allocInfo.commandBufferCount = 1;

//             vk::CommandBuffer commandBuffer = device->allocateCommandBuffers(allocInfo)[0];

//             vk::CommandBufferBeginInfo beginInfo = {};
//             beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;

//             commandBuffer.begin(beginInfo);

//             vk::BufferCopy copyRegion = {};
//             copyRegion.srcOffset = 0; // Optional
//             copyRegion.dstOffset = 0; // Optional
//             copyRegion.size = size;
//             commandBuffer.copyBuffer(srcBuffer, dstBuffer, copyRegion);

//             commandBuffer.end();

//             vk::SubmitInfo submitInfo = {};
//             submitInfo.commandBufferCount = 1;
//             submitInfo.pCommandBuffers = &commandBuffer;

//             graphicsQueue.submit(submitInfo, nullptr);
//             graphicsQueue.waitIdle();

//             device->freeCommandBuffers(commandPool, commandBuffer);
//         }

//         uint32_t VideoEngine::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
//         {
//             vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();

//             for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
//             {
//                 if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
//                 {
//                     return i;
//                 }
//             }

//             throw std::runtime_error("failed to find suitable memory type!");
//         }

//         void VideoEngine::createCommandBuffers()
//         {
//             commandBuffers.resize(swapChainFramebuffers.size());

//             vk::CommandBufferAllocateInfo allocInfo = {};
//             allocInfo.commandPool = commandPool;
//             allocInfo.level = vk::CommandBufferLevel::ePrimary;
//             allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

//             try
//             {
//                 commandBuffers = device->allocateCommandBuffers(allocInfo);
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to allocate command buffers!");
//             }

//             for (size_t i = 0; i < commandBuffers.size(); i++)
//             {
//                 vk::CommandBufferBeginInfo beginInfo = {};
//                 beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

//                 try
//                 {
//                     commandBuffers[i].begin(beginInfo);
//                 }
//                 catch (vk::SystemError err)
//                 {
//                     throw std::runtime_error("failed to begin recording command buffer!");
//                 }

//                 vk::RenderPassBeginInfo renderPassInfo = {};
//                 renderPassInfo.renderPass = renderPass;
//                 renderPassInfo.framebuffer = swapChainFramebuffers[i];
//                 renderPassInfo.renderArea.offset = vk::Offset2D{0, 0};
//                 renderPassInfo.renderArea.extent = swapChainExtent;

//                 vk::ClearValue clearColor = {std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f}};
//                 renderPassInfo.clearValueCount = 1;
//                 renderPassInfo.pClearValues = &clearColor;

//                 commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

//                 commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

//                 vk::Buffer vertexBuffers[] = {vertexBuffer};
//                 vk::DeviceSize offsets[] = {0};
//                 commandBuffers[i].bindVertexBuffers(0, 1, vertexBuffers, offsets);

//                 commandBuffers[i].draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);

//                 commandBuffers[i].endRenderPass();

//                 try
//                 {
//                     commandBuffers[i].end();
//                 }
//                 catch (vk::SystemError err)
//                 {
//                     throw std::runtime_error("failed to record command buffer!");
//                 }
//             }
//         }

//         void VideoEngine::createSyncObjects()
//         {
//             imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
//             renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
//             inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

//             try
//             {
//                 for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//                 {
//                     imageAvailableSemaphores[i] = device->createSemaphore({});
//                     renderFinishedSemaphores[i] = device->createSemaphore({});
//                     inFlightFences[i] = device->createFence({vk::FenceCreateFlagBits::eSignaled});
//                 }
//             }
//             catch (vk::SystemError err)
//             {
//                 throw std::runtime_error("failed to create synchronization objects for a frame!");
//             }
//         }

//         void VideoEngine::recreateSwapChain()
//         {
//             int width = 0, height = 0;
//             while (width == 0 || height == 0)
//             {
//                 SDL_Vulkan_GetDrawableSize(window, &width, &height);
//                 SDL_WaitEvent(event);
//             }

//             device->waitIdle();

//             cleanupSwapChain();

//             createSwapChain();
//             createImageViews();
//             createRenderPass();
//             createGraphicsPipeline();
//             createFramebuffers();
//             createCommandBuffers();
//         }

//         VkResult VideoEngine::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pCallback)
//         {
//             auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
//             if (func != nullptr)
//             {
//                 return func(instance, pCreateInfo, pAllocator, pCallback);
//             }
//             else
//             {
//                 return VK_ERROR_EXTENSION_NOT_PRESENT;
//             }
//         }

//         void VideoEngine::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks *pAllocator)
//         {
//             auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
//             if (func != nullptr)
//             {
//                 func(instance, callback, pAllocator);
//             }
//         }
//     }
// }