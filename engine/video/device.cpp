#include "device.hpp"
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace muyuy
{
    namespace video
    {

        Device::Device()
        {
            createInstance();
            setupDebugMessenger();
            createSurface();
            pickPhysicalDevice();
            createLogicalDevice();
        }
        void Device::cleanup()
        {
            device.destroy();
            instance.destroySurfaceKHR(surface);
            if (enableValidationLayers)
            {
                instance.destroyDebugUtilsMessengerEXT(debugUtilsMessenger);
            }
            instance.destroy();
        }
        void Device::createInstance()
        {
            vk::DynamicLoader loader;
            PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = loader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
            VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

            if (enableValidationLayers && !checkValidationLayerSupport())
            {
                throw std::runtime_error("validation layers requested, but not available!");
            }

            vk::ApplicationInfo appInfo{
                .pApplicationName = "Muyuy: the seventh heaven",
                .applicationVersion = 1,
                .pEngineName = "MuyuyEngine",
                .engineVersion = 1,
                .apiVersion = VK_API_VERSION_1_3};

            auto extensions = getRequiredExtensions();

            vk::InstanceCreateInfo createInfo{
                .flags = {},
                .pApplicationInfo = &appInfo,
                .enabledLayerCount = 0,
                .ppEnabledLayerNames = nullptr,
                .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
                .ppEnabledExtensionNames = extensions.data()};

            if (enableValidationLayers)
            {
                createInfo.setEnabledLayerCount(static_cast<uint32_t>(validationLayers.size()));
                createInfo.setPpEnabledLayerNames(validationLayers.data());
            }

            instance = vk::createInstance(createInfo);
            VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

            pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
            if (!pfnVkCreateDebugUtilsMessengerEXT)
            {
                std::cout << "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." << std::endl;
                exit(1);
            }

            pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
            if (!pfnVkDestroyDebugUtilsMessengerEXT)
            {
                std::cout << "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function." << std::endl;
                exit(1);
            }
        }

        bool Device::checkValidationLayerSupport()
        {
            std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

            for (const char *layerName : validationLayers)
            {
                bool layerFound = false;

                for (const auto &layerProperties : availableLayers)
                {
                    if (strcmp(layerName, layerProperties.layerName) == 0)
                    {
                        layerFound = true;
                        break;
                    }
                }

                if (!layerFound)
                {
                    return false;
                }
            }

            return true;
        }

        std::vector<const char *> Device::getRequiredExtensions()
        {
            unsigned int sdlExtensionCount = 0;
            SDL_Vulkan_GetInstanceExtensions(videoManager->window, &sdlExtensionCount, nullptr);
            std::vector<const char *> extensions(sdlExtensionCount);
            SDL_Vulkan_GetInstanceExtensions(videoManager->window, &sdlExtensionCount, extensions.data());

            if (enableValidationLayers)
            {
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }

            return extensions;
        }

        void Device::setupDebugMessenger()
        {
            if (!enableValidationLayers)
                return;

            vk::DebugUtilsMessengerCreateInfoEXT createInfo{
                .flags = {},
                .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
                .pfnUserCallback = &debugMessageFunc};

            debugUtilsMessenger = instance.createDebugUtilsMessengerEXT(createInfo);
        }

        void Device::createSurface()
        {
            VkSurfaceKHR rawSurface;
            if (SDL_Vulkan_CreateSurface(videoManager->window, instance, &rawSurface) == 0)
            {
                throw std::runtime_error("failed to create window surface!");
            }
            surface = rawSurface;
        }

        void Device::pickPhysicalDevice()
        {
            std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();

            for (const auto &device : devices)
            {
                if (isDeviceSuitable(device))
                {
                    physicalDevice = device;
                    break;
                }
            }

            if (!physicalDevice)
            {
                throw std::runtime_error("failed to find a suitable GPU!");
            }
        }

        bool Device::isDeviceSuitable(const vk::PhysicalDevice &device)
        {
            QueueFamilyIndices indices = findQueueFamilies(device);

            bool extensionsSupported = checkDeviceExtensionSupport(device);

            bool swapChainAdequate = false;
            if (extensionsSupported)
            {
                SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
                swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
            }

            vk::PhysicalDeviceFeatures supportedFeatures = device.getFeatures();

            return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
        }

        QueueFamilyIndices Device::findQueueFamilies(const vk::PhysicalDevice &device)
        {
            QueueFamilyIndices indices;

            std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

            int i = 0;
            for (const auto &queueFamily : queueFamilies)
            {
                if (queueFamily.queueFlags && VK_QUEUE_GRAPHICS_BIT)
                {
                    indices.graphicsFamily = i;
                }

                VkBool32 presentSupport = device.getSurfaceSupportKHR(i, surface);

                if (presentSupport)
                {
                    indices.presentFamily = i;
                }

                if (indices.isComplete())
                {
                    break;
                }

                i++;
            }

            return indices;
        }

        bool Device::checkDeviceExtensionSupport(const vk::PhysicalDevice &device)
        {
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();

            std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

            for (const auto &extension : availableExtensions)
            {
                requiredExtensions.erase(extension.extensionName);
            }

            return requiredExtensions.empty();
        }

        SwapChainSupportDetails Device::querySwapChainSupport(const vk::PhysicalDevice &device)
        {
            SwapChainSupportDetails details;

            details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
            details.formats = device.getSurfaceFormatsKHR(surface);
            details.presentModes = device.getSurfacePresentModesKHR(surface);

            return details;
        }

        void Device::createLogicalDevice()
        {
            QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

            std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
            std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

            float queuePriority = 1.0f;
            for (uint32_t queueFamily : uniqueQueueFamilies)
            {
                vk::DeviceQueueCreateInfo queueCreateInfo{
                    .flags = vk::DeviceQueueCreateFlags(),
                    .queueFamilyIndex = queueFamily,
                    .queueCount = 1,
                    .pQueuePriorities = &queuePriority};
                queueCreateInfos.push_back(queueCreateInfo);
            }

            vk::PhysicalDeviceFeatures deviceFeatures{
                .samplerAnisotropy = VK_TRUE};

            vk::DeviceCreateInfo createInfo{
                .flags = vk::DeviceCreateFlags(),
                .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
                .pQueueCreateInfos = queueCreateInfos.data(),
                .enabledLayerCount = enableValidationLayers ? static_cast<uint32_t>(validationLayers.size()) : 0,
                .ppEnabledLayerNames = enableValidationLayers ? validationLayers.data() : nullptr,
                .enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
                .ppEnabledExtensionNames = deviceExtensions.data(),
                .pEnabledFeatures = &deviceFeatures};

            device = physicalDevice.createDevice(createInfo);

            graphicsQueue = device.getQueue(indices.graphicsFamily.value(), 0);
            presentQueue = device.getQueue(indices.presentFamily.value(), 0);
        }
    }
}