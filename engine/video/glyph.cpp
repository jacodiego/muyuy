#include "glyph.hpp"

namespace muyuy::video
{

    Glyph::Glyph(FT_Face face, FT_UInt ix, std::shared_ptr<TextureAtlas> ta) : _face(face), _glyph_index(ix)
    {
        auto error = FT_Load_Glyph(_face, _glyph_index, FT_LOAD_DEFAULT);
        if (error)
        {
            throw std::runtime_error("failed to load glyph");
        }

        FT_GlyphSlot glyphSlot = _face->glyph;
        error = FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);
        if (error)
        {
            throw std::runtime_error("failed to render glyph");
        }

        _left = glyphSlot->bitmap_left;
        _top = glyphSlot->bitmap_top;
        _width = static_cast<int>(glyphSlot->metrics.width / 64);
        _height = static_cast<int>(glyphSlot->metrics.height / 64);
        _advance = static_cast<int>(glyphSlot->advance.x / 64);

        createTextureFromBitmap(ta);
    }

    void Glyph::createTextureFromBitmap(std::shared_ptr<TextureAtlas> &ta)
    {
        FT_GlyphSlot glyphSlot = _face->glyph;

        if (glyphSlot->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY || glyphSlot->bitmap.num_grays != 256)
        {
            throw std::runtime_error("unsupported pixel mode");
        }

        auto width = glyphSlot->bitmap.width;
        auto height = glyphSlot->bitmap.rows;
        auto bufferSize = width * height * 4;

        if (bufferSize == 0)
        {
            return;
        }

        std::vector<uint8_t> buffer(bufferSize);

        uint8_t *src = glyphSlot->bitmap.buffer;
        uint8_t *startOfLine = src;
        int dst = 0;

        for (int y = 0; y < height; ++y)
        {
            src = startOfLine;
            for (int x = 0; x < width; ++x)
            {
                auto value = *src;
                src++;

                buffer[dst++] = 0xff;
                buffer[dst++] = 0xff;
                buffer[dst++] = 0xff;
                buffer[dst++] = value;
            }
            startOfLine += glyphSlot->bitmap.pitch;
        }
        _texture = ta->add(width, height, buffer.data());
    }

    int Glyph::getLeft()
    {
        return _left;
    }

    int Glyph::getTop()
    {
        return _top;
    }

    std::shared_ptr<TextureBase> Glyph::getTexture()
    {
        return _texture;
    }

    int Glyph::getWidth()
    {
        return _width;
    }

    int Glyph::getHeight()
    {
        return _height;
    }

    int Glyph::getAdvance()
    {
        return _advance;
    }

}
