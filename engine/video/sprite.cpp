#include "sprite.hpp"

namespace muyuy::video
{
    Sprite::Sprite(TextureAtlas *owner, Area *area) : _owner(owner), _area(area)
    {
    }

    vk::DescriptorSet Sprite::getDescriptorSet(int i)
    {
        return _owner->getDescriptorSet(i);
    }

    TextureWindow Sprite::getTextureWindow()
    {
        float x0 = float(_area->x) / float(_owner->getWidth());
        float y0 = float(_area->y) / float(_owner->getHeight());
        float x1 = x0 + float(_area->width) / float(_owner->getWidth());
        float y1 = y0 + float(_area->height) / float(_owner->getHeight());
        return TextureWindow{x0, y0, x1, y1};
    }

    int Sprite::getWidth()
    {
        return _area->width;
    }

    int Sprite::getHeight()
    {
        return _area->height;
    }

}
