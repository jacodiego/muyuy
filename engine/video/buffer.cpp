#include "buffer.hpp"

namespace muyuy::video
{

    Buffer::Buffer(Device &dev) : device(dev)
    {
    }
    template <typename T>
    void Buffer::initialize(std::vector<T> b, vk::BufferUsageFlags usage)
    {
        vk::DeviceSize bufferSize = sizeof(b[0]) * b.size();

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, vk::BufferUsageFlagBits::eTransferSrc, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, stagingBuffer, stagingBufferMemory);

        void *data = device.getDevice().mapMemory(stagingBufferMemory, 0, bufferSize, 0);

        memcpy(data, b.data(), (size_t)bufferSize);
        device.getDevice().unmapMemory(stagingBufferMemory);

        // usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT

        createBuffer(bufferSize, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);

        copyBuffer(stagingBuffer, buffer, bufferSize);

        device.getDevice().destroyBuffer(stagingBuffer);
        device.getDevice().freeMemory(stagingBufferMemory);
    }

    void Buffer::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer &buffer, vk::DeviceMemory &bufferMem)
    {
        vk::BufferCreateInfo bufferInfo{
            .size = size,
            .usage = usage,
            .sharingMode = vk::SharingMode::eExclusive};

        buffer = device.getDevice().createBuffer(bufferInfo);

        vk::MemoryRequirements memRequirements = device.getDevice().getBufferMemoryRequirements(buffer);

        vk::MemoryAllocateInfo allocInfo{
            .allocationSize = memRequirements.size,
            .memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, properties)};

        bufferMem = device.getDevice().allocateMemory(allocInfo);

        device.getDevice().bindBufferMemory(buffer, bufferMem, 0);
    }

    void Buffer::copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size)
    {
        vk::CommandBuffer commandBuffer = device.beginSingleTimeCommands();

        vk::BufferCopy copyRegion{
            .size = size};

        commandBuffer.copyBuffer(srcBuffer, dstBuffer, copyRegion);

        device.endSingleTimeCommands(commandBuffer);
    }

}
