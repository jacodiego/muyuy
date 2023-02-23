#pragma once

#include "glyph.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>
#include <map>
#include <string>
#include <utility>

namespace muyuy::video
{

    struct TextDimensions
    {
        int width;
        int height;
    };

    class Texture;

    class Font
    {
    public:
        Font(const FT_Face &);

        int getNumGlyphs();
        Glyph *getGlyph(uint16_t);

        // TextDimensions measure(const std::string &);

    protected:
        FT_Face _face;
        std::map<uint16_t, Glyph *> _glyphs;
    };

}
