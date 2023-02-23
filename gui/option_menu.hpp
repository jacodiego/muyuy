#pragma once

#include "option.hpp"
#include "engine/video/video.hpp"

#include <string>
#include <vector>

namespace muyuy::gui
{
    class OptionMenu
    {
    public:
        OptionMenu();
        void setPosition(int, int);
        void setFont(video::FontTypes);
        void setColors(video::Color, video::Color);
        void addOption(std::string);
        void update();
        void draw();
        void moveLeft();
        void moveRight();
        int getSelected() { return _selected; };

    private:
        int _position_x, _position_y;
        std::vector<Option> _options;
        video::FontTypes _font;
        video::Color _options_color;
        video::Color _selected_color;
        int _selected;
        int _options_space;
    };
}
