#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace muyuy::video
{

    Texture::Texture(Device &dev, Renderer *ren) : device(dev),
                                                   renderer(ren),
                                                   width(0),
                                                   height(0),
                                                   _color(Color::White)
    {
    }

    void Texture::initialize(int w, int h, vk::Format format, vk::DescriptorPool descriptorPool, vk::DescriptorSetLayout descriptorSetLayout)
    {
        width = w;
        height = h;
        createImage(format, vk::ImageTiling::eLinear, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);

        transitionImageLayout(vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
        transitionImageLayout(vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

        createTextureImageView(format);
        createTextureSampler();
        createUniformBuffers();
        createDescriptorSets(descriptorPool, descriptorSetLayout);
    }

    void Texture::loadFromImage(const char *filepath, vk::DescriptorPool descriptorPool, vk::DescriptorSetLayout descriptorSetLayout)
    {
        int texChannels, w, h;
        stbi_uc *pixels = stbi_load(filepath, &w, &h, &texChannels, STBI_rgb_alpha);
        vk::DeviceSize imageSize = w * h * 4;

        if (!pixels)
        {
            throw std::runtime_error("failed to load texture image!");
        }

        initialize(w, h, vk::Format::eR8G8B8A8Srgb, descriptorPool, descriptorSetLayout);
        addPixels(pixels, 0, 0, w, h);
        stbi_image_free(pixels);
    }

    void Texture::addPixels(uint8_t *pixels, uint32_t offset_x, uint32_t offset_y, uint32_t w, uint32_t h)
    {
        buffer.initialize(pixels, w * h * 4);
        transitionImageLayout(vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
        buffer.copyBufferToImage(textureImage, static_cast<uint32_t>(w), static_cast<uint32_t>(h), offset_x, offset_y);
        transitionImageLayout(vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
        buffer.destroy();
    }

    void Texture::createImage(vk::Format format, vk::ImageTiling tiling, vk::ImageUsageFlags usage, vk::MemoryPropertyFlags properties)
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

    void Texture::transitionImageLayout(vk::ImageLayout oldLayout, vk::ImageLayout newLayout)
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

    void Texture::createTextureImageView(vk::Format format)
    {
        textureImageView = device.createImageView(textureImage, format);
    }

    void Texture::createTextureSampler()
    {

        vk::PhysicalDeviceProperties properties = device.getPhysicalDeviceProperties();

        vk::SamplerCreateInfo samplerInfo{
            .magFilter = vk::Filter::eLinear,
            .minFilter = vk::Filter::eLinear,
            .mipmapMode = vk::SamplerMipmapMode::eLinear,
            .addressModeU = vk::SamplerAddressMode::eRepeat,
            .addressModeV = vk::SamplerAddressMode::eRepeat,
            .addressModeW = vk::SamplerAddressMode::eRepeat,
            .mipLodBias = 0.0f,
            .anisotropyEnable = VK_TRUE,
            .maxAnisotropy = properties.limits.maxSamplerAnisotropy,
            .compareEnable = VK_FALSE,
            .compareOp = vk::CompareOp::eAlways,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = vk::BorderColor::eIntOpaqueBlack,
            .unnormalizedCoordinates = VK_FALSE};

        textureSampler = device.getDevice().createSampler(samplerInfo);
    }

    void Texture::createDescriptorSets(vk::DescriptorPool descriptorPool, vk::DescriptorSetLayout descriptorSetLayout)
    {
        std::vector<vk::DescriptorSetLayout> layouts(Swapchain::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
        vk::DescriptorSetAllocateInfo allocInfo{
            .descriptorPool = descriptorPool,
            .descriptorSetCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT),
            .pSetLayouts = layouts.data()};

        descriptorSets = device.getDevice().allocateDescriptorSets(allocInfo);

        for (size_t i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
        {
            vk::DescriptorBufferInfo bufferInfo{
                .buffer = uniformBuffers[i],
                .offset = 0,
                .range = sizeof(UniformBufferObject)};

            vk::DescriptorImageInfo imageInfo{
                .sampler = textureSampler,
                .imageView = textureImageView,
                .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal};

            vk::WriteDescriptorSet descUniformBuffers{
                .dstSet = descriptorSets[i],
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eUniformBuffer,
                .pBufferInfo = &bufferInfo,
            };

            vk::WriteDescriptorSet descImageSampler{
                .dstSet = descriptorSets[i],
                .dstBinding = 1,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eCombinedImageSampler,
                .pImageInfo = &imageInfo,
            };

            std::vector<vk::WriteDescriptorSet> descriptorWrites{descUniformBuffers, descImageSampler};
            device.getDevice().updateDescriptorSets(descriptorWrites, nullptr);
        }
    }

    void Texture::draw(ScreenPosition sp, int offset_x, int offset_y, int width, int height, float alpha, float scale, float multiplyColor)
    {
        int x, y;
        switch (sp)
        {
        case ScreenPosition::TopLeft:
            x = 0;
            y = 0;
            break;
        case ScreenPosition::TopCenter:
            x = device.getWindowExtent().width / 2 - width / 2;
            y = 0;
            break;
        case ScreenPosition::TopRight:
            x = device.getWindowExtent().width - width;
            y = 0;
            break;

        case ScreenPosition::MiddleLeft:
            x = 0;
            y = device.getWindowExtent().height / 2 - height / 2;
            break;
        case ScreenPosition::Center:
            x = device.getWindowExtent().width / 2 - width / 2;
            y = device.getWindowExtent().height / 2 - height / 2;
            break;
        case ScreenPosition::MiddleRight:
            x = device.getWindowExtent().width - width;
            y = device.getWindowExtent().height / 2 - height / 2;
            break;

        case ScreenPosition::BottomLeft:
            x = 0;
            y = device.getWindowExtent().height - height;
            break;
        case ScreenPosition::BottomCenter:
            x = device.getWindowExtent().width / 2 - width / 2;
            y = device.getWindowExtent().height - height;
            break;
        case ScreenPosition::BottomRight:
            x = device.getWindowExtent().width - width;
            y = device.getWindowExtent().height - height;
            break;

        default:
            x = 0;
            y = 0;
        }
        draw(x, y, offset_x, offset_y, width, height, alpha, scale, multiplyColor);
    }

    void Texture::draw(int x, int y, int offset_x, int offset_y, int width, int height, float alpha, float scale, float multiplyColor)
    {
        renderer->draw(this, x, y, offset_x, offset_y, width, height, alpha, scale, multiplyColor);
    }

    void Texture::createUniformBuffers()
    {
        vk::DeviceSize bufferSize = sizeof(UniformBufferObject);

        uniformBuffers.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMemory.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);
        uniformBuffersMapped.resize(Swapchain::MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
        {
            buffer.createBuffer(bufferSize, vk::BufferUsageFlagBits::eUniformBuffer, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, uniformBuffers[i], uniformBuffersMemory[i]);
            uniformBuffersMapped[i] = device.getDevice().mapMemory(uniformBuffersMemory[i], 0, bufferSize);
        }
    }

    void Texture::updateUniformBuffer(uint32_t currentImage, float alpha, float scale, float multiplyColor)
    {
        UniformBufferObject ubo{
            .alpha = alpha,
            .scale = scale,
            .multiplyColor = multiplyColor};

        memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

}
