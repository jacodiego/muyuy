#include "font_manager.hpp"

namespace muyuy::video
{

    FontManager::FontManager(std::shared_ptr<TextureAtlas> atlas) : _texture_atlas(std::move(atlas))
    {
        auto error = FT_Init_FreeType(&_library);
        if (error)
        {
            throw std::runtime_error("couldn't initialize FreeType library");
        }
    }

    std::shared_ptr<Font> FontManager::getFont(const std::string &fontname, int pt)
    {
        FT_Face face;
        auto error = FT_New_Face(_library, fontname.c_str(), 0, &face);
        if (!error)
        {
            error = FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(pt));
            if (error)
            {
                throw std::runtime_error("couldn't set pixel sizes");
            }
            return std::make_shared<Font>(face, _texture_atlas);
        }

        throw std::runtime_error("couldn't load font");
    }
}