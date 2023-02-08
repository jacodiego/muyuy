#pragma once

#include "texture_base.hpp"
#include "texture_atlas.hpp"

namespace muyuy::video
{

    struct Area
    {
        int x;
        int y;
        int width;
        int height;
    };

    class TextureAtlas;

    class Sprite : public TextureBase
    {
    public:
        Sprite(TextureAtlas *, Area *);

        vk::DescriptorSet getDescriptorSet(int) override;
        TextureWindow getTextureWindow() override;

        int getWidth() override;

        int getHeight() override;

    protected:
        TextureAtlas *_owner;
        Area *_area;
    };

}
