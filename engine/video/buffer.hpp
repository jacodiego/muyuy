#pragma once

#include "device.hpp"

#include <vector>

namespace muyuy::video
{
    class Buffer
    {
    public:
        Buffer(Device &);
        template <typename T>
        void initialize(std::vector<T>, vk::BufferUsageFlags);

    private:
        void createBuffer(vk::DeviceSize, vk::BufferUsageFlags, vk::MemoryPropertyFlags, vk::Buffer &, vk::DeviceMemory &);
        void copyBuffer(vk::Buffer, vk::Buffer, vk::DeviceSize);

    private:
        Device &device;
        vk::Buffer buffer;
        vk::DeviceMemory bufferMemory;
    };
}
