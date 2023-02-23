#include "buffer.hpp"

namespace muyuy::video
{

    Buffer::Buffer(Device &dev) : device(dev)
    {
    }

    void Buffer::destroy()
    {
        device.getDevice().destroyBuffer(buffer);
        device.getDevice().freeMemory(bufferMemory);
    }

    void Buffer::createBuffer(vk::DeviceSize size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties, vk::Buffer &buff, vk::DeviceMemory &bufferMem)
    {
        vk::BufferCreateInfo bufferInfo{
            .size = size,
            .usage = usage,
            .sharingMode = vk::SharingMode::eExclusive};

        buff = device.getDevice().createBuffer(bufferInfo);

        vk::MemoryRequirements memRequirements = device.getDevice().getBufferMemoryRequirements(buff);

        vk::MemoryAllocateInfo allocInfo{
            .allocationSize = memRequirements.size,
            .memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, properties)};

        bufferMem = device.getDevice().allocateMemory(allocInfo);

        device.getDevice().bindBufferMemory(buff, bufferMem, 0);
    }

    void Buffer::copyBuffer(vk::Buffer dstBuffer, vk::DeviceSize size)
    {
        vk::CommandBuffer commandBuffer = device.beginSingleTimeCommands();

        vk::BufferCopy copyRegion{
            .size = size};

        commandBuffer.copyBuffer(buffer, dstBuffer, copyRegion);

        device.endSingleTimeCommands(commandBuffer);
    }

    void Buffer::copyBufferToImage(vk::Image image, uint32_t width, uint32_t height, uint32_t offset_x, uint32_t offset_y)
    {
        vk::CommandBuffer commandBuffer = device.beginSingleTimeCommands();

        vk::BufferImageCopy region{
            .bufferOffset = 0,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .mipLevel = 0,
                .baseArrayLayer = 0,
                .layerCount = 1},
            .imageOffset = {offset_x, offset_y, 0},
            .imageExtent = {width, height, 1}};

        commandBuffer.copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, region);

        device.endSingleTimeCommands(commandBuffer);
    }

}
