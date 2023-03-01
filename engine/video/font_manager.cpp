#include "font_manager.hpp"

namespace muyuy::video
{

    FontManager::FontManager(Renderer *r) : renderer(r)
    {
        auto error = FT_Init_FreeType(&_library);
        if (error)
        {
            throw std::runtime_error("couldn't initialize FreeType library");
        }
    }

    void FontManager::addFont(FontTypes type, const std::string &fontname, int pt)
    {
        FT_Face face;
        if (FT_New_Face(_library, fontname.c_str(), 0, &face))
        {
            throw std::runtime_error("couldn't load font");
        }
        if (FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(pt)))
        {
            throw std::runtime_error("couldn't set pixel sizes");
        }
        _fonts.insert(std::pair<FontTypes, Font *>(type, new Font(face)));
    }

    void FontManager::write(Texture *texture, FontTypes type, const std::string &text)
    {
        int pos = 0;
        const int fontsize = 36;
        int y = fontsize;
        texture->initialize(textWidth(type, text), fontsize * 1.5, vk::Format::eR8G8B8A8Unorm, renderer->getDescriptorPool(poolTypes::Global), renderer->getDescriptorSetLayout(descriptorTypes::Sampler));
        for (auto c : text)
        {
            auto glyph = _fonts.at(type)->getGlyph(c);

            // if (pos + glyph->getAdvance() >= texture->getWidth())
            // {
            //     pos = 0;
            //     y += fontsize;
            // }
            if (glyph->getBufferBitmap() != NULL)
            {
                float x0 = pos + glyph->getLeft();
                float y0 = y - glyph->getTop();
                texture->addPixels(glyph->getBufferBitmap(), x0, y0, glyph->getWidth(), glyph->getHeight());
            }
            pos += glyph->getAdvance();
        }
    }

    int FontManager::textWidth(FontTypes type, const std::string &text)
    {
        int pos = 0;
        for (auto c : text)
        {
            auto glyph = _fonts.at(type)->getGlyph(c);
            pos += glyph->getAdvance();
        }
        pos++;
        return pos;
    }
}