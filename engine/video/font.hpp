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

    class TextureAtlas;
    class Renderer;

    class Font
    {
    public:
        Font(const FT_Face &, std::shared_ptr<TextureAtlas>);

        int getNumGlyphs();
        std::shared_ptr<Glyph> getGlyph(uint16_t);

        TextDimensions measure(const std::string &);
        void draw(Renderer &, float, float, const std::string &);

    protected:
        FT_Face _face;
        std::shared_ptr<TextureAtlas> _texture_atlas;
        std::map<uint16_t, std::shared_ptr<Glyph>> _glyphs;
    };

}
