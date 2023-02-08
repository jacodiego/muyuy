#pragma once

#include "texture_base.hpp"
#include "sprite.hpp"

#include <memory>

namespace muyuy::video
{

    class Sprite;
    class Renderer;

    class TextureAtlas : public TextureBase
    {
    public:
        TextureAtlas();

        ~TextureAtlas();

        void initialize(Renderer *, uint32_t, uint32_t);

        std::shared_ptr<Sprite> add(const std::string &);

        std::shared_ptr<Sprite> add(uint32_t, uint32_t, uint8_t *);

        vk::DescriptorSet getDescriptorSet(int i) override { return _descriptor_sets[i]; };
        TextureWindow getTextureWindow() override;
        int getWidth() override;
        int getHeight() override;

    protected:
        uint32_t _width;
        uint32_t _height;
        Renderer *_renderer;
        // BoundImage _boundImage;
        vk::ImageView _imageView;
        vk::Sampler _sampler;
        std::vector<vk::DescriptorSet> _descriptor_sets;
        // AreaAllocator _allocator;
    };

}
