#include "texture_array.hpp"

namespace muyuy::video
{

    TextureArray::TextureArray(Device &dev, std::vector<TextureMap> texture_maps,
                               vk::DescriptorPool descriptorPool, vk::DescriptorSetLayout descriptorSetLayout) : device(dev),
                                                                                                                 _texture_maps(texture_maps)
    {
        createTextureSampler();
        createDescriptorSets(descriptorPool, descriptorSetLayout);
    }

    TextureArray::~TextureArray()
    {
    }

    void TextureArray::createTextureSampler()
    {

        vk::PhysicalDeviceProperties properties = device.getPhysicalDeviceProperties();

        vk::SamplerCreateInfo samplerInfo{
            .magFilter = vk::Filter::eNearest,
            .minFilter = vk::Filter::eNearest,
            .mipmapMode = vk::SamplerMipmapMode::eLinear,
            .addressModeU = vk::SamplerAddressMode::eClampToEdge,
            .addressModeV = vk::SamplerAddressMode::eClampToEdge,
            .addressModeW = vk::SamplerAddressMode::eClampToEdge,
            .mipLodBias = 0.0f,
            .anisotropyEnable = VK_TRUE,
            .maxAnisotropy = properties.limits.maxSamplerAnisotropy,
            .compareEnable = VK_FALSE,
            .compareOp = vk::CompareOp::eAlways,
            .minLod = 0.0f,
            .maxLod = 0.0f,
            .borderColor = vk::BorderColor::eIntOpaqueBlack,
            .unnormalizedCoordinates = VK_FALSE};

        _sampler = device.getDevice().createSampler(samplerInfo);
    }

    void TextureArray::createDescriptorSets(vk::DescriptorPool descriptorPool, vk::DescriptorSetLayout descriptorSetLayout)
    {
        std::vector<vk::DescriptorSetLayout> layouts(Swapchain::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
        vk::DescriptorSetAllocateInfo allocInfo{
            .descriptorPool = descriptorPool,
            .descriptorSetCount = static_cast<uint32_t>(Swapchain::MAX_FRAMES_IN_FLIGHT),
            .pSetLayouts = layouts.data()};

        descriptorSets = device.getDevice().allocateDescriptorSets(allocInfo);

        for (size_t i = 0; i < Swapchain::MAX_FRAMES_IN_FLIGHT; i++)
        {
            vk::DescriptorImageInfo imageInfo[TEXTURE_ARRAY_LENGTH];
            for (auto texture_map : _texture_maps)
            {
                imageInfo[texture_map.index] = vk::DescriptorImageInfo{
                    .sampler = nullptr,
                    .imageView = texture_map.texture->getImageView(),
                    .imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal};
            }

            vk::DescriptorImageInfo samplerImageInfo{
                .sampler = _sampler};

            vk::WriteDescriptorSet descImageSampler{
                .dstSet = descriptorSets[i],
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorCount = 1,
                .descriptorType = vk::DescriptorType::eSampler,
                .pImageInfo = &samplerImageInfo,
            };

            vk::WriteDescriptorSet descImageTexture{
                .dstSet = descriptorSets[i],
                .dstBinding = 1,
                .dstArrayElement = 0,
                .descriptorCount = TEXTURE_ARRAY_LENGTH,
                .descriptorType = vk::DescriptorType::eSampledImage,
                .pImageInfo = imageInfo,
            };

            std::vector<vk::WriteDescriptorSet> descriptorWrites{descImageSampler, descImageTexture};
            device.getDevice().updateDescriptorSets(descriptorWrites, nullptr);
        }
    }
}
