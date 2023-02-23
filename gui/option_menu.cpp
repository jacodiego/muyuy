#include "option_menu.hpp"

namespace muyuy::gui
{
    OptionMenu::OptionMenu() : _selected(0),
                               _options_space(70),
                               _options_color(video::Color::White),
                               _selected_color(video::Color::Black) {}

    void OptionMenu::setPosition(int x, int y)
    {
        _position_x = x;
        _position_y = y;
    }

    void OptionMenu::setFont(video::FontTypes font)
    {
        _font = font;
    }

    void OptionMenu::setColors(video::Color option, video::Color selected)
    {
        _options_color = option;
        _selected_color = selected;
    }

    void OptionMenu::addOption(std::string label)
    {
        _options.push_back(Option{label, _font});
    }

    void OptionMenu::moveLeft()
    {
        if (_selected > 0)
        {
            _selected = _selected - 1;
        }
    }

    void OptionMenu::moveRight()
    {
        if (_selected < _options.size() - 1)
        {
            _selected = _selected + 1;
        }
    }

    void OptionMenu::update()
    {
        for (int i = 0; i < _options.size(); i++)
        {
            _options[i].setColor(i == _selected ? _selected_color : _options_color);
        }
    }

    void OptionMenu::draw()
    {
        int x = _position_x;
        for (auto option : _options)
        {
            option.draw(x, _position_y);
            x = x + option.getWidth() + _options_space;
        }
    }

}