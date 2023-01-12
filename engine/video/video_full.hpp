// #pragma once

// #include "utils/singleton.hpp"

// #include <vulkan/vulkan.h>
// #include <SDL2/SDL_vulkan.h>
// #include <SDL2/SDL.h>
// #include <glm/glm.hpp>

// #include <iostream>
// #include <stdexcept>
// #include <cstdlib>
// #include <vector>
// #include <optional>
// #include <set>
// #include <limits>
// #include <algorithm>
// #include <fstream>
// #include <array>

// #define GLM_FORCE_RADIANS
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// #include <chrono>

// namespace muyuy
// {
//     namespace video
//     {

//         class VideoEngine;
//         extern VideoEngine *videoManager;

//         struct UniformBufferObject
//         {
//             alignas(16) glm::mat4 model;
//             alignas(16) glm::mat4 view;
//             alignas(16) glm::mat4 proj;
//         };

//         struct Vertex
//         {
//             glm::vec2 pos;
//             glm::vec3 color;
//             glm::vec2 texCoord;

//             static VkVertexInputBindingDescription getBindingDescription()
//             {
//                 VkVertexInputBindingDescription bindingDescription{};
//                 bindingDescription.binding = 0;
//                 bindingDescription.stride = sizeof(Vertex);
//                 bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

//                 return bindingDescription;
//             }

//             static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
//             {
//                 std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

//                 attributeDescriptions[0].binding = 0;
//                 attributeDescriptions[0].location = 0;
//                 attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
//                 attributeDescriptions[0].offset = offsetof(Vertex, pos);

//                 attributeDescriptions[1].binding = 0;
//                 attributeDescriptions[1].location = 1;
//                 attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//                 attributeDescriptions[1].offset = offsetof(Vertex, color);

//                 attributeDescriptions[2].binding = 0;
//                 attributeDescriptions[2].location = 2;
//                 attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
//                 attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

//                 return attributeDescriptions;
//             }
//         };

//         struct QueueFamilyIndices
//         {
//             std::optional<uint32_t> graphicsFamily;
//             std::optional<uint32_t> presentFamily;

//             bool isComplete()
//             {
//                 return graphicsFamily.has_value() && presentFamily.has_value();
//             }
//         };

//         struct SwapChainSupportDetails
//         {
//             VkSurfaceCapabilitiesKHR capabilities;
//             std::vector<VkSurfaceFormatKHR> formats;
//             std::vector<VkPresentModeKHR> presentModes;
//         };

//         class VideoEngine : public utils::Singleton<VideoEngine>
//         {
//             friend class utils::Singleton<VideoEngine>;
//             const int MAX_FRAMES_IN_FLIGHT = 2;
//             const std::vector<const char *> validationLayers = {
//                 "VK_LAYER_KHRONOS_validation"};
//             const std::vector<const char *> deviceExtensions = {
//                 VK_KHR_SWAPCHAIN_EXTENSION_NAME};

//             // const std::vector<Vertex> vertices = {
//             //     {{-0.5f, -0.5f}, {1.0f, 0.3f, 0.7f}, {1.0f, 0.0f}},
//             //     {{0.5f, -0.5f}, {0.7f, 1.0f, 0.3f}, {0.0f, 0.0f}},
//             //     {{0.5f, 0.5f}, {0.7f, 0.3f, 1.0f}, {0.0f, 1.0f}},
//             //     {{-0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}};

//             const std::vector<Vertex> vertices = {
//                 {{-1.0f, -1.0f}, {1.0f, 0.3f, 0.7f}, {1.0f, 0.0f}},
//                 {{1.0f, -1.0f}, {0.7f, 1.0f, 0.3f}, {0.0f, 0.0f}},
//                 {{1.0f, 1.0f}, {0.7f, 0.3f, 1.0f}, {0.0f, 1.0f}},
//                 {{-1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}};

//             const std::vector<uint16_t> indices = {
//                 0, 1, 2, 2, 3, 0};

// #ifdef NDEBUG
//             const bool enableValidationLayers = false;
// #else
//             const bool enableValidationLayers = true;
// #endif

//             static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
//                 VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//                 VkDebugUtilsMessageTypeFlagsEXT messageType,
//                 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
//                 void *pUserData)
//             {

//                 std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

//                 return VK_FALSE;
//             }

//             static std::vector<char> readFile(const std::string &filename)
//             {
//                 std::ifstream file(filename, std::ios::ate | std::ios::binary);

//                 if (!file.is_open())
//                 {
//                     throw std::runtime_error("failed to open file!");
//                 }

//                 size_t fileSize = (size_t)file.tellg();
//                 std::vector<char> buffer(fileSize);

//                 file.seekg(0);
//                 file.read(buffer.data(), fileSize);

//                 file.close();

//                 return buffer;
//             }

//         public:
//             bool singletonInitialize();
//             void initVulkan(SDL_Window *, SDL_Event *);
//             void drawFrame();
//             void waitIdle();
//             void resizeScreen(int, int);
//             void cleanup();

//         private:
//             void createInstance();
//             void setupDebugMessenger();
//             void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &);
//             bool checkValidationLayerSupport();
//             void pickPhysicalDevice();
//             std::vector<const char *> getRequiredExtensions();
//             VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *, const VkAllocationCallbacks *, VkDebugUtilsMessengerEXT *);
//             void DestroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks *);
//             bool isDeviceSuitable(VkPhysicalDevice);
//             QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);
//             void createLogicalDevice();
//             void createSurface();
//             bool checkDeviceExtensionSupport(VkPhysicalDevice);
//             SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice);
//             VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &);
//             VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &);
//             VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &);
//             void createSwapChain();
//             void createImageViews();
//             void createGraphicsPipeline();
//             VkShaderModule createShaderModule(const std::vector<char> &);
//             void createRenderPass();
//             void createFramebuffers();
//             void createCommandPool();
//             void createCommandBuffers();
//             void recordCommandBuffer(VkCommandBuffer, uint32_t);
//             void createSyncObjects();
//             void recreateSwapChain();
//             void cleanupSwapChain();
//             void createTextureImage();
//             void createVertexBuffer();
//             uint32_t findMemoryType(uint32_t, VkMemoryPropertyFlags);
//             void createBuffer(VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags, VkBuffer &, VkDeviceMemory &);
//             void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize);
//             void createIndexBuffer();
//             void createDescriptorSetLayout();
//             void createUniformBuffers();
//             void updateUniformBuffer(uint32_t);
//             void createDescriptorPool();
//             void createDescriptorSets();
//             void createImage(uint32_t, uint32_t, VkFormat, VkImageTiling, VkImageUsageFlags, VkMemoryPropertyFlags, VkImage &, VkDeviceMemory &);
//             VkCommandBuffer beginSingleTimeCommands();
//             void endSingleTimeCommands(VkCommandBuffer commandBuffer);
//             void transitionImageLayout(VkImage, VkFormat, VkImageLayout, VkImageLayout);
//             void copyBufferToImage(VkBuffer, VkImage, uint32_t, uint32_t);
//             void createTextureImageView();
//             VkImageView createImageView(VkImage, VkFormat);
//             void createTextureSampler();
//             SDL_Window *window;
//             SDL_Event *event;
//             VkInstance instance;
//             VkDebugUtilsMessengerEXT debugMessenger;
//             VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
//             VkDevice device;
//             VkQueue graphicsQueue;
//             VkQueue presentQueue;
//             VkSurfaceKHR surface;
//             VkSwapchainKHR swapChain = VK_NULL_HANDLE;
//             VkSwapchainKHR oldSwapChain = VK_NULL_HANDLE;
//             std::vector<VkImage> swapChainImages;
//             VkFormat swapChainImageFormat;
//             VkExtent2D swapChainExtent;
//             std::vector<VkImageView> swapChainImageViews;
//             VkRenderPass renderPass;
//             VkDescriptorSetLayout descriptorSetLayout;
//             VkPipelineLayout pipelineLayout;
//             VkPipeline graphicsPipeline;
//             std::vector<VkFramebuffer> swapChainFramebuffers;
//             VkCommandPool commandPool;
//             std::vector<VkCommandBuffer> commandBuffers;
//             std::vector<VkSemaphore> imageAvailableSemaphores;
//             std::vector<VkSemaphore> renderFinishedSemaphores;
//             std::vector<VkFence> inFlightFences;
//             bool framebufferResized = false;
//             uint32_t currentFrame = 0;
//             VkBuffer vertexBuffer;
//             VkDeviceMemory vertexBufferMemory;
//             VkBuffer indexBuffer;
//             VkDeviceMemory indexBufferMemory;
//             std::vector<VkBuffer> uniformBuffers;
//             std::vector<VkDeviceMemory> uniformBuffersMemory;
//             std::vector<void *> uniformBuffersMapped;
//             VkDescriptorPool descriptorPool;
//             std::vector<VkDescriptorSet> descriptorSets;
//             VkImage textureImage;
//             VkDeviceMemory textureImageMemory;
//             VkImageView textureImageView;
//             VkSampler textureSampler;
//         };
//     }
// }