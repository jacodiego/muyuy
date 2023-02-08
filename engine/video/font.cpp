#include "font.hpp"

namespace muyuy::video
{

    Font::Font(const FT_Face &face, std::shared_ptr<TextureAtlas> ta) : _face(face), _texture_atlas(std::move(ta))
    {
    }

    int Font::getNumGlyphs()
    {
        return static_cast<int>(_face->num_glyphs);
    }

    std::shared_ptr<Glyph> Font::getGlyph(uint16_t c)
    {
        auto foundIt = _glyphs.find(c);
        if (foundIt != _glyphs.end())
        {
            return foundIt->second;
        }

        auto glyphIndex = FT_Get_Char_Index(_face, c);
        auto glyph = std::make_shared<Glyph>(_face, glyphIndex, _texture_atlas);

        _glyphs[c] = glyph;
        return glyph;
    }

    TextDimensions Font::measure(const std::string &text)
    {
        int width = 0;
        int height = 0;

        for (auto c : text)
        {
            auto glyph = getGlyph(c);
            width += glyph->getAdvance();
            height = std::max(height, glyph->getTop() + glyph->getHeight());
        }

        return TextDimensions{width, height};
    }

    void Font::draw(Renderer &r, float x, float y, const std::string &text)
    {
        int pos = x;

        for (auto c : text)
        {
            auto glyph = getGlyph(c);
            auto texture = glyph->getTexture();
            if (texture)
            {
                // r.setTexture(texture);
                float x0 = pos + glyph->getLeft();
                float y0 = y - glyph->getTop();
                // r.drawSprite(x0, y0, texture->getWidth(), texture->getHeight());
            }
            pos += glyph->getAdvance();
        }
    }

}
