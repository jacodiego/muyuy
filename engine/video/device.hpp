#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>
#include <SDL2/SDL_vulkan.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <array>
#include <optional>
#include <set>

namespace muyuy::video
{
    enum class ScreenPosition
    {
        TopLeft,
        TopCenter,
        TopRight,
        MiddleLeft,
        Center,
        MiddleRight,
        BottomLeft,
        BottomCenter,
        BottomRight,
    };

    const int MAX_FRAMES_IN_FLIGHT = 2;

    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME, VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    static std::vector<char> readFile(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete()
        {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };
    class Device
    {
        PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
        PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;

        VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                                      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                                                      const VkAllocationCallbacks *pAllocator,
                                                                      VkDebugUtilsMessengerEXT *pMessenger)
        {
            return pfnVkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, pAllocator, pMessenger);
        }

        VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, VkAllocationCallbacks const *pAllocator)
        {
            return pfnVkDestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
        }

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                               VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                                                               VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                                                               void * /*pUserData*/)
        {
            std::ostringstream message;

            message << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << ": "
                    << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) << ":\n";
            message << std::string("\t") << "messageIDName   = <" << pCallbackData->pMessageIdName << ">\n";
            message << std::string("\t") << "messageIdNumber = " << pCallbackData->messageIdNumber << "\n";
            message << std::string("\t") << "message         = <" << pCallbackData->pMessage << ">\n";
            if (0 < pCallbackData->queueLabelCount)
            {
                message << std::string("\t") << "Queue Labels:\n";
                for (uint32_t i = 0; i < pCallbackData->queueLabelCount; i++)
                {
                    message << std::string("\t\t") << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
                }
            }
            if (0 < pCallbackData->cmdBufLabelCount)
            {
                message << std::string("\t") << "CommandBuffer Labels:\n";
                for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
                {
                    message << std::string("\t\t") << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
                }
            }
            if (0 < pCallbackData->objectCount)
            {
                message << std::string("\t") << "Objects:\n";
                for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
                {
                    message << std::string("\t\t") << "Object " << i << "\n";
                    message << std::string("\t\t\t") << "objectType   = " << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)) << "\n";
                    message << std::string("\t\t\t") << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
                    if (pCallbackData->pObjects[i].pObjectName)
                    {
                        message << std::string("\t\t\t") << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
                    }
                }
            }

#ifdef _WIN32
            MessageBox(NULL, message.str().c_str(), "Alert", MB_OK);
#else
            std::cout << message.str() << std::endl;
#endif

            return false;
        }

    public:
        Device();
        void initialize(SDL_Window *);
        bool singletonInitialize() { return true; };
        vk::Device getDevice() { return device; };
        vk::SurfaceKHR getSurface() { return surface; };
        vk::CommandPool getCommandPool() { return commandPool; };
        vk::Queue getGraphicsQueue() { return graphicsQueue; };
        vk::Queue getPresentQueue() { return presentQueue; };
        SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
        QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
        uint32_t findMemoryType(uint32_t, vk::MemoryPropertyFlags);
        vk::CommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(vk::CommandBuffer);
        void waitIdle();
        vk::Extent2D getWindowExtent();
        vk::ImageView createImageView(vk::Image image, vk::Format format);
        vk::PhysicalDeviceProperties getPhysicalDeviceProperties();
        void cleanup();

    private:
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        bool checkValidationLayerSupport();
        std::vector<const char *> getRequiredExtensions();
        vk::DebugUtilsMessengerCreateInfoEXT createDebugMessengerCreateInfo();
        bool isDeviceSuitable(const vk::PhysicalDevice &);
        QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice &);
        bool checkDeviceExtensionSupport(const vk::PhysicalDevice &);
        SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice &);
        void createCommandPool();

    private:
        SDL_Window *window;
        vk::Instance instance;
        vk::DebugUtilsMessengerEXT debugUtilsMessenger;
        vk::SurfaceKHR surface;

        vk::PhysicalDevice physicalDevice;
        vk::Device device;

        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        vk::CommandPool commandPool;
    };

}
