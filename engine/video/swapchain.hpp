#pragma once

#define VULKAN_HPP_NO_CONSTRUCTORS
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include <vulkan/vulkan.hpp>

#include "device.hpp"

namespace muyuy::video
{
    class Device;
    class Swapchain
    {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 2;
        Swapchain(Device &);
        vk::Extent2D getSwapChainExtent() { return swapChainExtent; };
        vk::RenderPass getRenderPass() { return renderPass; };

    private:
        void initialize();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();
        vk::ImageView createImageView(vk::Image image, vk::Format format);
        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &);
        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &);
        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &);

    private:
        Device &device;
        vk::SwapchainKHR swapChain = VK_NULL_HANDLE;
        vk::SwapchainKHR oldSwapChain = VK_NULL_HANDLE;
        std::vector<vk::Image> swapChainImages;
        vk::Format swapChainImageFormat;
        vk::Extent2D swapChainExtent;
        std::vector<vk::ImageView> swapChainImageViews;
        std::vector<vk::Framebuffer> swapChainFramebuffers;
        vk::RenderPass renderPass;
        std::vector<vk::Semaphore> imageAvailableSemaphores;
        std::vector<vk::Semaphore> renderFinishedSemaphores;
        std::vector<vk::Fence> inFlightFences;
    };
}
