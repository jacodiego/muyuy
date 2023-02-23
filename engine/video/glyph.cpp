#include "glyph.hpp"

namespace muyuy::video
{

    Glyph::Glyph(FT_Face face, FT_UInt ix) : _face(face), _glyph_index(ix)
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

        createBufferBitmap();
    }

    void Glyph::createBufferBitmap()
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

        buffer.resize(bufferSize);

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
    }

    int Glyph::getLeft()
    {
        return _left;
    }

    int Glyph::getTop()
    {
        return _top;
    }

    int Glyph::getWidth()
    {
        return _width;
    }

    int Glyph::getHeight()
    {
        return _height;
    }

    int Glyph::getBitmapWidth()
    {
        FT_GlyphSlot glyphSlot = _face->glyph;
        return glyphSlot->bitmap.width;
    }

    int Glyph::getBitmapHeight()
    {
        FT_GlyphSlot glyphSlot = _face->glyph;
        return glyphSlot->bitmap.rows;
    }

    int Glyph::getAdvance()
    {
        return _advance;
    }

    uint8_t *Glyph::getBufferBitmap()
    {
        return buffer.data();
    }

}
