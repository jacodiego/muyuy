#include "font.hpp"

namespace muyuy::video
{

    Font::Font(const FT_Face &face) : _face(face)
    {
    }

    int Font::getNumGlyphs()
    {
        return static_cast<int>(_face->num_glyphs);
    }

    Glyph *Font::getGlyph(uint16_t c)
    {
        auto foundIt = _glyphs.find(c);
        if (foundIt != _glyphs.end())
        {
            return foundIt->second;
        }

        auto glyphIndex = FT_Get_Char_Index(_face, c);
        auto glyph = new Glyph(_face, glyphIndex);

        _glyphs[c] = glyph;
        return glyph;
    }

    // TextDimensions Font::measure(const std::string &text)
    // {
    //     int width = 0;
    //     int height = 0;

    //     for (auto c : text)
    //     {
    //         auto glyph = getGlyph(c);
    //         width += glyph->getAdvance();
    //         height = std::max(height, glyph->getTop() + glyph->getHeight());
    //     }

    //     return TextDimensions{width, height};
    // }

}
