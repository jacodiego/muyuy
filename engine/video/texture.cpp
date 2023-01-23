#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace muyuy::video
{

    Texture::Texture(Device &dev) : device(dev)
    {
    }

    void Texture::load(const char *filepath)
    {

        int texWidth, texHeight, texChannels;
        stbi_uc *pixels = stbi_load(filepath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        vk::DeviceSize imageSize = texWidth * texHeight * 4;

        if (!pixels)
        {
            throw std::runtime_error("failed to load texture image!");
        }

        vk::Buffer stagingBuffer;
        vk::DeviceMemory stagingBufferMemory;

        // createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        // void *data;
        // vkMapMemory(device, stagingBufferMemory, 0, imageSize, 0, &data);
        // memcpy(data, pixels, static_cast<size_t>(imageSize));
        // vkUnmapMemory(device, stagingBufferMemory);

        stbi_image_free(pixels);

        createImage(texWidth, texHeight, vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);
        transitionImageLayout(vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
        copyBufferToImage(stagingBuffer, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        transitionImageLayout(vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

        device.getDevice().destroyBuffer(stagingBuffer);
        device.getDevice().freeMemory(stagingBufferMemory);
    }

    void Texture::createImage(uint32_t width, uint32_t height, vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties)
    {
        vk::ImageCreateInfo imageInfo{
            .imageType = vk::ImageType::e2D,
            .format = format,
            .extent = {width, height, 1},
            .mipLevels = 1,
            .arrayLayers = 1,
            .samples = vk::SampleCountFlagBits::e1,
            .tiling = tiling,
            .usage = usage,
            .sharingMode = vk::SharingMode::eExclusive,
            .initialLayout = vk::ImageLayout::eUndefined};

        textureImage = device.getDevice().createImage(imageInfo);

        vk::MemoryRequirements memRequirements = device.getDevice().getImageMemoryRequirements(textureImage);

        vk::MemoryAllocateInfo allocInfo{
            .allocationSize = memRequirements.size,
            .memoryTypeIndex = device.findMemoryType(memRequirements.memoryTypeBits, properties)};

        textureImageMemory = device.getDevice().allocateMemory(allocInfo);

        device.getDevice().bindImageMemory(textureImage, textureImageMemory, 0);
    }

    void Texture::transitionImageLayout(vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
    {
        vk::CommandBuffer commandBuffer = device.beginSingleTimeCommands();

        vk::PipelineStageFlags sourceStage;
        vk::PipelineStageFlags destinationStage;
        vk::AccessFlagBits srcAccessMask;
        vk::AccessFlagBits dstAccessMask;

        if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal)
        {
            srcAccessMask = vk::AccessFlagBits::eNone;
            dstAccessMask = vk::AccessFlagBits::eTransferWrite;
            sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
            destinationStage = vk::PipelineStageFlagBits::eTransfer;
        }
        else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal)
        {
            srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            dstAccessMask = vk::AccessFlagBits::eShaderRead;
            sourceStage = vk::PipelineStageFlagBits::eTransfer;
            destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
        }
        else
        {
            throw std::invalid_argument("unsupported layout transition!");
        }

        vk::ImageMemoryBarrier barrier{
            .srcAccessMask = srcAccessMask,
            .dstAccessMask = dstAccessMask,
            .oldLayout = oldLayout,
            .newLayout = newLayout,
            .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
            .image = textureImage,
            .subresourceRange = {
                .aspectMask = vk::ImageAspectFlagBits::eColor,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1}};

        commandBuffer.pipelineBarrier(sourceStage, destinationStage, vk::DependencyFlags(), nullptr, nullptr, barrier);

        device.endSingleTimeCommands(commandBuffer);
    }

    void Texture::copyBufferToImage(vk::Buffer buffer, uint32_t width, uint32_t height)
    {
        vk::CommandBuffer commandBuffer = device.beginSingleTimeCommands();

        vk::BufferImageCopy region{
            .bufferOffset = 0,
            .bufferRowLength = 0,
            .bufferImageHeight = 0,
            .imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor,
            .imageSubresource.mipLevel = 0,
            .imageSubresource.baseArrayLayer = 0,
            .imageSubresource.layerCount = 1,
            .imageOffset = {0, 0, 0},
            .imageExtent = {width, height, 1}};

        commandBuffer.copyBufferToImage(buffer, textureImage, vk::ImageLayout::eTransferDstOptimal, region);

        device.endSingleTimeCommands(commandBuffer);
    }

    void Texture::createTextureImageView()
    {
        textureImageView = device.createImageView(textureImage, vk::Format::eR8G8B8A8Srgb);
    }

    void Texture::createTextureSampler()
    {

        vk::PhysicalDeviceProperties properties = device.getPhysicalDeviceProperties();

        vk::SamplerCreateInfo samplerInfo{
            .magFilter = vk::Filter::eLinear,
            .minFilter = vk::Filter::eLinear,
            .addressModeU = vk::SamplerAddressMode::eRepeat,
            .addressModeV = vk::SamplerAddressMode::eRepeat,
            .addressModeW = vk::SamplerAddressMode::eRepeat,
            .anisotropyEnable = VK_TRUE,
            .maxAnisotropy = properties.limits.maxSamplerAnisotropy,
            .borderColor = vk::BorderColor::eIntOpaqueBlack,
            .unnormalizedCoordinates = VK_FALSE,
            .compareEnable = VK_FALSE,
            .compareOp = vk::CompareOp::eAlways,
            .mipmapMode = vk::SamplerMipmapMode::eLinear,
            .mipLodBias = 0.0f,
            .minLod = 0.0f,
            .maxLod = 0.0f};

        textureSampler = device.getDevice().createSampler(samplerInfo);
    }

}
