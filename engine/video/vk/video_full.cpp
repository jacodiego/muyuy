// #include "video_full.hpp"
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb/stb_image.h>

//         void VideoEngine::initVulkan(SDL_Window *win, SDL_Event *ev)
//         {
//             window = win;
//             event = ev;
//             createInstance();
//             setupDebugMessenger();
//             createSurface();
//             pickPhysicalDevice();
//             createLogicalDevice();
//             createSwapChain();
//             createImageViews();
//             createRenderPass();
//             createDescriptorSetLayout();
//             createGraphicsPipeline();
//             createFramebuffers();
//             createCommandPool();
//             createTextureImage();
//             createTextureImageView();
//             createTextureSampler();
//             createVertexBuffer();
//             createIndexBuffer();
//             createUniformBuffers();
//             createDescriptorPool();
//             createDescriptorSets();
//             createCommandBuffers();
//             createSyncObjects();
//         }

//         void VideoEngine::drawFrame()
//         {
//             vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

//             uint32_t imageIndex;
//             VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

//             if (result == VK_ERROR_OUT_OF_DATE_KHR)
//             {
//                 recreateSwapChain();
//                 return;
//             }
//             else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
//             {
//                 throw std::runtime_error("failed to acquire swap chain image!");
//             }

//             vkResetFences(device, 1, &inFlightFences[currentFrame]);
//             vkResetCommandBuffer(commandBuffers[currentFrame], 0);
//             recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

//             updateUniformBuffer(currentFrame);

//             VkSubmitInfo submitInfo{};
//             submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

//             VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
//             VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
//             submitInfo.waitSemaphoreCount = 1;
//             submitInfo.pWaitSemaphores = waitSemaphores;
//             submitInfo.pWaitDstStageMask = waitStages;
//             submitInfo.commandBufferCount = 1;
//             submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

//             VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
//             submitInfo.signalSemaphoreCount = 1;
//             submitInfo.pSignalSemaphores = signalSemaphores;

//             if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to submit draw command buffer!");
//             }

//             VkPresentInfoKHR presentInfo{};
//             presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

//             presentInfo.waitSemaphoreCount = 1;
//             presentInfo.pWaitSemaphores = signalSemaphores;

//             VkSwapchainKHR swapChains[] = {swapChain};
//             presentInfo.swapchainCount = 1;
//             presentInfo.pSwapchains = swapChains;
//             presentInfo.pImageIndices = &imageIndex;

//             result = vkQueuePresentKHR(presentQueue, &presentInfo);

//             if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
//             {
//                 framebufferResized = false;
//                 recreateSwapChain();
//             }
//             else if (result != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to present swap chain image!");
//             }

//             currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
//         }

//         void VideoEngine::resizeScreen(int w, int h)
//         {
//             framebufferResized = true;
//         }

//         void VideoEngine::cleanup()
//         {
//             cleanupSwapChain();

//             vkDestroySampler(device, textureSampler, nullptr);
//             vkDestroyImageView(device, textureImageView, nullptr);
//             vkDestroyImage(device, textureImage, nullptr);
//             vkFreeMemory(device, textureImageMemory, nullptr);

//             for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//             {
//                 vkDestroyBuffer(device, uniformBuffers[i], nullptr);
//                 vkFreeMemory(device, uniformBuffersMemory[i], nullptr);
//             }

//             vkDestroyDescriptorPool(device, descriptorPool, nullptr);
//             vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);

//             vkDestroyBuffer(device, indexBuffer, nullptr);
//             vkFreeMemory(device, indexBufferMemory, nullptr);

//             vkDestroyBuffer(device, vertexBuffer, nullptr);
//             vkFreeMemory(device, vertexBufferMemory, nullptr);

//             vkDestroyPipeline(device, graphicsPipeline, nullptr);
//             vkDestroyPipelineLayout(device, pipelineLayout, nullptr);

//             vkDestroyRenderPass(device, renderPass, nullptr);

//             for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//             {
//                 vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
//                 vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
//                 vkDestroyFence(device, inFlightFences[i], nullptr);
//             }

//             vkDestroyCommandPool(device, commandPool, nullptr);

//             vkDestroySwapchainKHR(device, swapChain, nullptr);
//             vkDestroyDevice(device, nullptr);

//             if (enableValidationLayers)
//             {
//                 DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
//             }

//             vkDestroySurfaceKHR(instance, surface, nullptr);
//             vkDestroyInstance(instance, nullptr);
//         }

//         void VideoEngine::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
//         {
//             VkCommandBufferBeginInfo beginInfo{};
//             beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//             beginInfo.flags = 0;                  // Optional
//             beginInfo.pInheritanceInfo = nullptr; // Optional

//             if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to begin recording command buffer!");
//             }

//             VkRenderPassBeginInfo renderPassInfo{};
//             renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//             renderPassInfo.renderPass = renderPass;
//             renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];
//             renderPassInfo.renderArea.offset = {0, 0};
//             renderPassInfo.renderArea.extent = swapChainExtent;

//             VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
//             renderPassInfo.clearValueCount = 1;
//             renderPassInfo.pClearValues = &clearColor;

//             vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

//             VkViewport viewport{};
//             viewport.x = 0.0f;
//             viewport.y = 0.0f;
//             viewport.width = static_cast<float>(swapChainExtent.width);
//             viewport.height = static_cast<float>(swapChainExtent.height);
//             viewport.minDepth = 0.0f;
//             viewport.maxDepth = 1.0f;
//             vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

//             VkRect2D scissor{};
//             scissor.offset = {0, 0};
//             scissor.extent = swapChainExtent;
//             vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

//             vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

//             VkBuffer vertexBuffers[] = {vertexBuffer};
//             VkDeviceSize offsets[] = {0};
//             vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

//             vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

//             vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
//             vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

//             vkCmdEndRenderPass(commandBuffer);
//             if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to record command buffer!");
//             }
//         }

//         void VideoEngine::cleanupSwapChain()
//         {
//             for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
//             {
//                 vkDestroyFramebuffer(device, swapChainFramebuffers[i], nullptr);
//             }

//             for (size_t i = 0; i < swapChainImageViews.size(); i++)
//             {
//                 vkDestroyImageView(device, swapChainImageViews[i], nullptr);
//             }

//             vkDestroySwapchainKHR(device, oldSwapChain, nullptr);
//         }

//         void VideoEngine::createVertexBuffer()
//         {
//             VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

//             VkBuffer stagingBuffer;
//             VkDeviceMemory stagingBufferMemory;
//             createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

//             void *data;
//             vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
//             memcpy(data, vertices.data(), (size_t)bufferSize);
//             vkUnmapMemory(device, stagingBufferMemory);

//             createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

//             copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

//             vkDestroyBuffer(device, stagingBuffer, nullptr);
//             vkFreeMemory(device, stagingBufferMemory, nullptr);
//         }

//         uint32_t VideoEngine::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
//         {
//             VkPhysicalDeviceMemoryProperties memProperties;
//             vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
//             for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
//             {
//                 if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
//                 {
//                     return i;
//                 }
//             }

//             throw std::runtime_error("failed to find suitable memory type!");
//         }

//         void VideoEngine::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory)
//         {
//             VkBufferCreateInfo bufferInfo{};
//             bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//             bufferInfo.size = size;
//             bufferInfo.usage = usage;
//             bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//             if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to create buffer!");
//             }

//             VkMemoryRequirements memRequirements;
//             vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

//             VkMemoryAllocateInfo allocInfo{};
//             allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//             allocInfo.allocationSize = memRequirements.size;
//             allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

//             if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to allocate buffer memory!");
//             }

//             vkBindBufferMemory(device, buffer, bufferMemory, 0);
//         }

//         void VideoEngine::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
//         {
//             VkCommandBuffer commandBuffer = beginSingleTimeCommands();

//             VkBufferCopy copyRegion{};
//             copyRegion.size = size;
//             vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

//             endSingleTimeCommands(commandBuffer);
//         }

//         void VideoEngine::createIndexBuffer()
//         {
//             VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

//             VkBuffer stagingBuffer;
//             VkDeviceMemory stagingBufferMemory;
//             createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

//             void *data;
//             vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
//             memcpy(data, indices.data(), (size_t)bufferSize);
//             vkUnmapMemory(device, stagingBufferMemory);

//             createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

//             copyBuffer(stagingBuffer, indexBuffer, bufferSize);

//             vkDestroyBuffer(device, stagingBuffer, nullptr);
//             vkFreeMemory(device, stagingBufferMemory, nullptr);
//         }

//         void VideoEngine::createDescriptorSetLayout()
//         {
//             VkDescriptorSetLayoutBinding uboLayoutBinding{};
//             uboLayoutBinding.binding = 0;
//             uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//             uboLayoutBinding.descriptorCount = 1;
//             uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//             uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

//             VkDescriptorSetLayoutBinding samplerLayoutBinding{};
//             samplerLayoutBinding.binding = 1;
//             samplerLayoutBinding.descriptorCount = 1;
//             samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//             samplerLayoutBinding.pImmutableSamplers = nullptr;
//             samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

//             std::array<VkDescriptorSetLayoutBinding, 2> bindings = {uboLayoutBinding, samplerLayoutBinding};
//             VkDescriptorSetLayoutCreateInfo layoutInfo{};
//             layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
//             layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
//             layoutInfo.pBindings = bindings.data();

//             if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to create descriptor set layout!");
//             }
//         }

//         void VideoEngine::createUniformBuffers()
//         {
//             VkDeviceSize bufferSize = sizeof(UniformBufferObject);

//             uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
//             uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
//             uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

//             for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//             {
//                 createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

//                 vkMapMemory(device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
//             }
//         }

//         void VideoEngine::updateUniformBuffer(uint32_t currentImage)
//         {
//             static auto startTime = std::chrono::high_resolution_clock::now();

//             auto currentTime = std::chrono::high_resolution_clock::now();
//             float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

//             UniformBufferObject ubo{};
//             ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//             ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//             ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
//             ubo.proj[1][1] *= -1;

//             memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
//         }

//         void VideoEngine::createDescriptorPool()
//         {
//             std::array<VkDescriptorPoolSize, 2> poolSizes{};
//             poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//             poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//             poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//             poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

//             VkDescriptorPoolCreateInfo poolInfo{};
//             poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
//             poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
//             poolInfo.pPoolSizes = poolSizes.data();
//             poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

//             if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to create descriptor pool!");
//             }
//         }

//         void VideoEngine::createDescriptorSets()
//         {
//             std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
//             VkDescriptorSetAllocateInfo allocInfo{};
//             allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
//             allocInfo.descriptorPool = descriptorPool;
//             allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
//             allocInfo.pSetLayouts = layouts.data();

//             descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
//             if (vkAllocateDescriptorSets(device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to allocate descriptor sets!");
//             }

//             for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
//             {
//                 VkDescriptorBufferInfo bufferInfo{};
//                 bufferInfo.buffer = uniformBuffers[i];
//                 bufferInfo.offset = 0;
//                 bufferInfo.range = sizeof(UniformBufferObject);

//                 VkDescriptorImageInfo imageInfo{};
//                 imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
//                 imageInfo.imageView = textureImageView;
//                 imageInfo.sampler = textureSampler;

//                 std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

//                 descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//                 descriptorWrites[0].dstSet = descriptorSets[i];
//                 descriptorWrites[0].dstBinding = 0;
//                 descriptorWrites[0].dstArrayElement = 0;
//                 descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//                 descriptorWrites[0].descriptorCount = 1;
//                 descriptorWrites[0].pBufferInfo = &bufferInfo;

//                 descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
//                 descriptorWrites[1].dstSet = descriptorSets[i];
//                 descriptorWrites[1].dstBinding = 1;
//                 descriptorWrites[1].dstArrayElement = 0;
//                 descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
//                 descriptorWrites[1].descriptorCount = 1;
//                 descriptorWrites[1].pImageInfo = &imageInfo;

//                 vkUpdateDescriptorSets(device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
//             }
//         }

//         void VideoEngine::createTextureImage()
//         {
//             int texWidth, texHeight, texChannels;
//             stbi_uc *pixels = stbi_load("data/boot/logo.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
//             VkDeviceSize imageSize = texWidth * texHeight * 4;

//             if (!pixels)
//             {
//                 throw std::runtime_error("failed to load texture image!");
//             }

//             VkBuffer stagingBuffer;
//             VkDeviceMemory stagingBufferMemory;

//             createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
//             void *data;
//             vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
//             memcpy(data, pixels, static_cast<size_t>(imageSize));
//             vkUnmapMemory(device, stagingBufferMemory);
//             stbi_image_free(pixels);

//             createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);
//             transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
//             copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
//             transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
//             vkDestroyBuffer(device, stagingBuffer, nullptr);
//             vkFreeMemory(device, stagingBufferMemory, nullptr);
//         }

//         void VideoEngine::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory)
//         {
//             VkImageCreateInfo imageInfo{};
//             imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
//             imageInfo.imageType = VK_IMAGE_TYPE_2D;
//             imageInfo.extent.width = width;
//             imageInfo.extent.height = height;
//             imageInfo.extent.depth = 1;
//             imageInfo.mipLevels = 1;
//             imageInfo.arrayLayers = 1;
//             imageInfo.format = format;
//             imageInfo.tiling = tiling;
//             imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
//             imageInfo.usage = usage;
//             imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
//             imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//             if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to create image!");
//             }

//             VkMemoryRequirements memRequirements;
//             vkGetImageMemoryRequirements(device, image, &memRequirements);

//             VkMemoryAllocateInfo allocInfo{};
//             allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//             allocInfo.allocationSize = memRequirements.size;
//             allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

//             if (vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to allocate image memory!");
//             }

//             vkBindImageMemory(device, image, imageMemory, 0);
//         }

//         VkCommandBuffer VideoEngine::beginSingleTimeCommands()
//         {
//             VkCommandBufferAllocateInfo allocInfo{};
//             allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
//             allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
//             allocInfo.commandPool = commandPool;
//             allocInfo.commandBufferCount = 1;

//             VkCommandBuffer commandBuffer;
//             vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

//             VkCommandBufferBeginInfo beginInfo{};
//             beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//             beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

//             vkBeginCommandBuffer(commandBuffer, &beginInfo);

//             return commandBuffer;
//         }

//         void VideoEngine::endSingleTimeCommands(VkCommandBuffer commandBuffer)
//         {
//             vkEndCommandBuffer(commandBuffer);

//             VkSubmitInfo submitInfo{};
//             submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//             submitInfo.commandBufferCount = 1;
//             submitInfo.pCommandBuffers = &commandBuffer;

//             vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
//             vkQueueWaitIdle(graphicsQueue);

//             vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
//         }

//         void VideoEngine::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
//         {
//             VkCommandBuffer commandBuffer = beginSingleTimeCommands();

//             VkImageMemoryBarrier barrier{};
//             barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//             barrier.oldLayout = oldLayout;
//             barrier.newLayout = newLayout;
//             barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//             barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//             barrier.image = image;
//             barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//             barrier.subresourceRange.baseMipLevel = 0;
//             barrier.subresourceRange.levelCount = 1;
//             barrier.subresourceRange.baseArrayLayer = 0;
//             barrier.subresourceRange.layerCount = 1;
//             VkPipelineStageFlags sourceStage;
//             VkPipelineStageFlags destinationStage;

//             if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
//             {
//                 barrier.srcAccessMask = 0;
//                 barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

//                 sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
//                 destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
//             }
//             else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
//             {
//                 barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//                 barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

//                 sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
//                 destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
//             }
//             else
//             {
//                 throw std::invalid_argument("unsupported layout transition!");
//             }

//             vkCmdPipelineBarrier(
//                 commandBuffer,
//                 sourceStage, destinationStage,
//                 0,
//                 0, nullptr,
//                 0, nullptr,
//                 1, &barrier);

//             endSingleTimeCommands(commandBuffer);
//         }

//         void VideoEngine::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
//         {
//             VkCommandBuffer commandBuffer = beginSingleTimeCommands();

//             VkBufferImageCopy region{};
//             region.bufferOffset = 0;
//             region.bufferRowLength = 0;
//             region.bufferImageHeight = 0;

//             region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//             region.imageSubresource.mipLevel = 0;
//             region.imageSubresource.baseArrayLayer = 0;
//             region.imageSubresource.layerCount = 1;

//             region.imageOffset = {0, 0, 0};
//             region.imageExtent = {
//                 width,
//                 height,
//                 1};

//             vkCmdCopyBufferToImage(
//                 commandBuffer,
//                 buffer,
//                 image,
//                 VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
//                 1,
//                 &region);

//             endSingleTimeCommands(commandBuffer);
//         }

//         void VideoEngine::createTextureImageView()
//         {
//             textureImageView = createImageView(textureImage, VK_FORMAT_R8G8B8A8_SRGB);
//         }

//         void VideoEngine::createTextureSampler()
//         {
//             VkSamplerCreateInfo samplerInfo{};
//             samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
//             samplerInfo.magFilter = VK_FILTER_LINEAR;
//             samplerInfo.minFilter = VK_FILTER_LINEAR;
//             samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//             samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//             samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
//             samplerInfo.anisotropyEnable = VK_TRUE;
//             VkPhysicalDeviceProperties properties{};
//             vkGetPhysicalDeviceProperties(physicalDevice, &properties);
//             samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
//             samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
//             samplerInfo.unnormalizedCoordinates = VK_FALSE;
//             samplerInfo.compareEnable = VK_FALSE;
//             samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
//             samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
//             samplerInfo.mipLodBias = 0.0f;
//             samplerInfo.minLod = 0.0f;
//             samplerInfo.maxLod = 0.0f;

//             if (vkCreateSampler(device, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
//             {
//                 throw std::runtime_error("failed to create texture sampler!");
//             }
//         }
//     }
// }