// #pragma once

// #include "utils/singleton.hpp"

// #include <vulkan/vulkan.hpp>
// #include <SDL2/SDL_vulkan.h>
// #include <SDL2/SDL.h>
// #include <glm/glm.hpp>

// #include <iostream>
// #include <fstream>
// #include <stdexcept>
// #include <algorithm>
// #include <vector>
// #include <cstring>
// #include <cstdlib>
// #include <array>
// #include <optional>
// #include <set>

// namespace muyuy
// {
//     namespace video
//     {
//         class VideoEngine;
//         extern VideoEngine *videoManager;

//         const int MAX_FRAMES_IN_FLIGHT = 2;

//         const std::vector<const char *> validationLayers = {
//             "VK_LAYER_KHRONOS_validation"};

//         const std::vector<const char *> deviceExtensions = {
//             VK_KHR_SWAPCHAIN_EXTENSION_NAME};

// #ifdef NDEBUG
//         const bool enableValidationLayers = false;
// #else
//         const bool enableValidationLayers = true;
// #endif

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
//             vk::SurfaceCapabilitiesKHR capabilities;
//             std::vector<vk::SurfaceFormatKHR> formats;
//             std::vector<vk::PresentModeKHR> presentModes;
//         };

//         struct Vertex
//         {
//             glm::vec2 pos;
//             glm::vec3 color;

//             static vk::VertexInputBindingDescription getBindingDescription()
//             {
//                 vk::VertexInputBindingDescription bindingDescription = {};
//                 bindingDescription.binding = 0;
//                 bindingDescription.stride = sizeof(Vertex);
//                 bindingDescription.inputRate = vk::VertexInputRate::eVertex;

//                 return bindingDescription;
//             }

//             static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions()
//             {
//                 std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions = {};
//                 attributeDescriptions[0].binding = 0;
//                 attributeDescriptions[0].location = 0;
//                 attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
//                 attributeDescriptions[0].offset = offsetof(Vertex, pos);

//                 attributeDescriptions[1].binding = 0;
//                 attributeDescriptions[1].location = 1;
//                 attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
//                 attributeDescriptions[1].offset = offsetof(Vertex, color);

//                 return attributeDescriptions;
//             }
//         };

//         const std::vector<Vertex> vertices = {
//             {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
//             {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
//             {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

//         static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData)
//         {
//             std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

//             return VK_FALSE;
//         }

//         static std::vector<char> readFile(const std::string &filename)
//         {
//             std::ifstream file(filename, std::ios::ate | std::ios::binary);

//             if (!file.is_open())
//             {
//                 throw std::runtime_error("failed to open file!");
//             }

//             size_t fileSize = (size_t)file.tellg();
//             std::vector<char> buffer(fileSize);

//             file.seekg(0);
//             file.read(buffer.data(), fileSize);

//             file.close();

//             return buffer;
//         }
//         class VideoEngine : public utils::Singleton<VideoEngine>
//         {
//             friend class utils::Singleton<VideoEngine>;

//         public:
//             bool singletonInitialize();
//             void initialize(SDL_Window *, SDL_Event *);
//             void drawFrame();
//             void waitIdle();
//             void resizeScreen(int, int);
//             void cleanup();

//         private:
//             void createInstance();
//             void setupDebugCallback();
//             void createSurface();
//             void pickPhysicalDevice();
//             void createLogicalDevice();
//             void createSwapChain();
//             void createImageViews();
//             void createRenderPass();
//             void createGraphicsPipeline();
//             void createFramebuffers();
//             void createCommandPool();
//             void createVertexBuffer();
//             void createCommandBuffers();
//             void createSyncObjects();
//             bool checkValidationLayerSupport();
//             std::vector<const char *> getRequiredExtensions();
//             bool isDeviceSuitable(const vk::PhysicalDevice &);
//             QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice);
//             bool checkDeviceExtensionSupport(const vk::PhysicalDevice &);
//             SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice &);
//             vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &);
//             vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>);
//             vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &);
//             vk::UniqueShaderModule createShaderModule(const std::vector<char> &);
//             void createBuffer(vk::DeviceSize size, vk::BufferUsageFlags, vk::MemoryPropertyFlags, vk::Buffer &, vk::DeviceMemory &);
//             void copyBuffer(VkBuffer, VkBuffer, VkDeviceSize);
//             uint32_t findMemoryType(uint32_t, vk::MemoryPropertyFlags);
//             void cleanupSwapChain();
//             void recreateSwapChain();
//             void createDescriptorSetLayout();
//             VkResult CreateDebugUtilsMessengerEXT(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT *, const VkAllocationCallbacks *, VkDebugUtilsMessengerEXT *);
//             void DestroyDebugUtilsMessengerEXT(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks *);

//         private:
//             SDL_Window *window;
//             SDL_Event *event;
//             vk::UniqueInstance instance;
//             VkDebugUtilsMessengerEXT callback;
//             vk::SurfaceKHR surface;

//             vk::PhysicalDevice physicalDevice;
//             vk::UniqueDevice device;

//             vk::Queue graphicsQueue;
//             vk::Queue presentQueue;

//             vk::SwapchainKHR swapChain;
//             std::vector<vk::Image> swapChainImages;
//             vk::Format swapChainImageFormat;
//             vk::Extent2D swapChainExtent;
//             std::vector<vk::ImageView> swapChainImageViews;
//             std::vector<vk::Framebuffer> swapChainFramebuffers;

//             vk::RenderPass renderPass;
//             vk::UniqueDescriptorSetLayout descriptorSetLayout;
//             vk::PipelineLayout pipelineLayout;
//             vk::Pipeline graphicsPipeline;

//             vk::CommandPool commandPool;

//             vk::Buffer vertexBuffer;
//             vk::DeviceMemory vertexBufferMemory;

//             std::vector<vk::CommandBuffer, std::allocator<vk::CommandBuffer>> commandBuffers;

//             std::vector<vk::Semaphore> imageAvailableSemaphores;
//             std::vector<vk::Semaphore> renderFinishedSemaphores;
//             std::vector<vk::Fence> inFlightFences;
//             size_t currentFrame = 0;

//             bool framebufferResized = false;
//         };

//     }
// }
