#include "texture_atlas.hpp"
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb/stb_image.h>

namespace muyuy::video
{
    TextureAtlas::TextureAtlas() : _width(0), _height(0), _renderer(nullptr)
    {
    }

    TextureAtlas::~TextureAtlas()
    {
        // _renderer->destroyImageView(_imageView);
        // _renderer->destroySampler(_sampler);
        // _renderer->destroyImage(_boundImage);
    }

    void TextureAtlas::initialize(Renderer *pRenderer, uint32_t width, uint32_t height)
    {
        _renderer = pRenderer;

        _width = width;
        _height = height;

        // VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
        // _boundImage = _renderer->createImage(
        //     _width, _height,
        //     format, VK_IMAGE_TILING_OPTIMAL,
        //     VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        //     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        // _renderer->transitionImageLayout(
        //     _boundImage.image,
        //     VK_IMAGE_LAYOUT_UNDEFINED,
        //     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        // _renderer->transitionImageLayout(
        //     _boundImage.image,
        //     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        //     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        // _imageView = _renderer->createImageView(_boundImage.image, format);
        // _sampler = _renderer->createSampler();
        // _descriptorSet = _renderer->createTextureDescriptorSet(_imageView, _sampler);

        // _allocator.initialize(_width, _height);
    }

    int TextureAtlas::getWidth()
    {
        return _width;
    }

    int TextureAtlas::getHeight()
    {
        return _height;
    }

    std::shared_ptr<Sprite> TextureAtlas::add(const std::string &filename)
    {
        // int width;
        // int height;
        // int channels;
        // stbi_uc *pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        // if (!pixels)
        // {
        //     throw std::runtime_error("failed to load texture image");
        // }

        // auto result = add(static_cast<uint32_t>(width), static_cast<uint32_t>(height), pixels);

        // stbi_image_free(pixels);

        // return result;
        return nullptr;
    }

    std::shared_ptr<Sprite> TextureAtlas::add(uint32_t width, uint32_t height, uint8_t *pixels)
    {
        // auto area = _allocator.allocate(width, height);
        // if (!area)
        // {
        //     return nullptr;
        // }

        // VkDeviceSize imageSize = width * height * 4;
        // auto stagingBuffer = _renderer->createBuffer(
        //     imageSize,
        //     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        //     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

        // _renderer->copyToBufferMemory(stagingBuffer.bufferMemory, pixels, imageSize);

        // _renderer->transitionImageLayout(
        //     _boundImage.image,
        //     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        //     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        // _renderer->copyBufferToImage(stagingBuffer.buffer, _boundImage.image, width, height, area->x, area->y);
        // _renderer->transitionImageLayout(
        //     _boundImage.image,
        //     VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        //     VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        // _renderer->destroyBufferLater(stagingBuffer);

        // return std::make_shared<Sprite>(this, area);
        return nullptr;
    }

    TextureWindow TextureAtlas::getTextureWindow()
    {
        return TextureWindow{0.0f, 0.0f, 1.0f, 1.0f};
    }

}
