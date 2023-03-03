#pragma once

#include "device.hpp"

#include <vector>

namespace muyuy::video
{
    class Buffer
    {
        friend class Texture;
        friend class TextureArray;
        friend class Renderer;

    public:
        Buffer(Device &);
        template <typename T>
        void initialize(T b, vk::DeviceSize bufferSize)
        {
            createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, buffer, bufferMemory);

            void *data = device.getDevice().mapMemory(bufferMemory, 0, bufferSize);

            memcpy(data, b, (size_t)bufferSize);
            device.getDevice().unmapMemory(bufferMemory);
        };
        void destroy();

    private:
        void createBuffer(vk::DeviceSize, vk::BufferUsageFlags, vk::MemoryPropertyFlags, vk::Buffer &, vk::DeviceMemory &);
        void copyBuffer(vk::Buffer, vk::DeviceSize);
        void copyBufferToImage(vk::Image, uint32_t, uint32_t, uint32_t, uint32_t);

    private:
        Device &device;
        vk::Buffer buffer;
        vk::DeviceMemory bufferMemory;
    };
}
