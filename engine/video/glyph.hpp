#pragma once

#include "texture_atlas.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <stdexcept>
#include <vector>

namespace muyuy::video
{

    class TextureBase;

    class Glyph
    {
    public:
        Glyph(FT_Face, FT_UInt, std::shared_ptr<TextureAtlas>);

        int getLeft();
        int getTop();
        int getWidth();
        int getHeight();
        int getAdvance();

        std::shared_ptr<TextureBase> getTexture();

    protected:
        FT_Face _face;
        FT_UInt _glyph_index;
        int _left;
        int _top;
        int _width;
        int _height;
        int _advance;
        std::shared_ptr<TextureBase> _texture;

        void createTextureFromBitmap(std::shared_ptr<TextureAtlas> &);
    };

}
