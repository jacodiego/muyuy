#pragma once

#include "engine/video/video.hpp"
#include <string>

namespace muyuy::gui
{
    class Option
    {
    public:
        Option(std::string &, video::FontTypes);
        void setColor(video::Color);
        int getWidth();
        void draw(int, int);

    private:
        std::string _label;
        video::Texture *_texture;
    };
}
