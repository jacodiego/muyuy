#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace muyuy::video
{

    Texture::Texture(Device &dev, Renderer *ren) : device(dev), renderer(ren)
    {
    }

    void Texture::load(const char *filepath, vk::DescriptorPool descriptorPool, vk::DescriptorSetLayout descriptorSetLayout)
    {

        int texChannels;
        stbi_uc *pixels = stbi_load(filepath, &width, &height, &texChannels, STBI_rgb_alpha);
        vk::DeviceSize imageSize = width * height * 4;

        if (!pixels)
        {
            throw std::runtime_error("failed to load texture image!");
        }

        buffer.initialize(pixels, imageSize);

        stbi_image_free(pixels);

        createImage(vk::Format::eR8G8B8A8Srgb, vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled, vk::MemoryPropertyFlagBits::eDeviceLocal);
        transitionImageLayout(vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
        buffer.copyBufferToImage(textureImage, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
        transitionImageLayout(vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

        buffer.destroy();

        createTextureImageView();
        createTextureSampler();
        createUniformBuffers();
        createDescriptorSets(descriptorPool, descriptorSetLayout);

        // viewport = vk::Viewport{
        //     .x = (float)renderer->getWindowExtent().width / 2 - (float)width / 2,
        //     .y = (float)renderer->getWindowExtent().height / 2 - (float)height / 2,
        //     .width = static_cast<float>(width),
        //     .height = static_cast<float>(height),
        //     .minDepth = 0.0f,
        //     .maxDepth = 1.0f};
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

    void Texture::draw(ScreenPosition sp, int offset_x, int offset_y, int width, int height, float alpha, float scale)
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
        draw(x, y, offset_x, offset_y, width, height, alpha, scale);
    }

    void Texture::draw(int x, int y, int offset_x, int offset_y, int width, int height, float alpha, float scale)
    {
        renderer->draw(this, x, y, offset_x, offset_y, width, height, alpha, scale);
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

    void Texture::updateUniformBuffer(uint32_t currentImage, float alpha, float scale)
    {
        UniformBufferObject ubo{
            .alpha = alpha,
            .scale = scale};

        memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

    TextureWindow Texture::getTextureWindow()
    {
        return TextureWindow{0.0f, 0.0f, 1.0f, 1.0f};
    }

}
