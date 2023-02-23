#pragma once

#include "texture.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <stdexcept>
#include <vector>

namespace muyuy::video
{

    class Glyph
    {
    public:
        Glyph(FT_Face, FT_UInt);

        int getLeft();
        int getTop();
        int getWidth();
        int getHeight();
        int getBitmapWidth();
        int getBitmapHeight();
        int getAdvance();
        void createBufferBitmap();
        uint8_t *getBufferBitmap();

    protected:
        FT_Face _face;
        FT_UInt _glyph_index;
        std::vector<uint8_t> buffer;
        int _left;
        int _top;
        int _width;
        int _height;
        int _advance;
    };

}
