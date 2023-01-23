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

//     }
// }