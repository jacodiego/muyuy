#pragma once

#include "font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <utility>
#include <memory>
#include <string>

namespace muyuy::video
{

    class TextureAtlas;

    class FontManager
    {
    public:
        explicit FontManager(std::shared_ptr<TextureAtlas>);

        std::shared_ptr<Font> getFont(const std::string &, int);

    protected:
        FT_Library _library;
        std::shared_ptr<TextureAtlas> _texture_atlas;
    };

}
