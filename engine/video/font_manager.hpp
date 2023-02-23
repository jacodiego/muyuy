#pragma once

#include "font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <utility>
#include <memory>
#include <string>
#include <map>

namespace muyuy::video
{

    class Texture;
    class Renderer;

    enum class FontTypes
    {
        UnboundedXs,
        UnboundedSm,
        UnboundedMd,
        UnboundedLg,
        UnboundedXl,
        MontserratRegular
    };

    class FontManager
    {
    public:
        FontManager(Renderer *);
        void addFont(FontTypes, const std::string &, int);
        void write(Texture *, FontTypes, const std::string &);
        int textWidth(FontTypes, const std::string &);
        // int textHeight(const std::string &);

    protected:
        Renderer *renderer;
        FT_Library _library;
        std::map<FontTypes, Font *> _fonts;
    };

}
