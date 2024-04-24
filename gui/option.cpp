#include "option.hpp"

namespace muyuy::gui
{
    Option::Option(std::string &label, video::FontTypes font) : _label(label)
    {
        _texture = video::videoManager->createTexture();
        video::videoManager->write(_texture, font, label);
    }

    Option::~Option() {
        // delete _texture;
    }

    void Option::setColor(video::Color color)
    {
        _texture->setColor(color);
    }

    int Option::getWidth()
    {
        return _texture->getWidth();
    }

    void Option::draw(int x, int y)
    {
        _texture->draw(x, y, 0, 0, _texture->getWidth(), _texture->getHeight(), 1.0f, 1.0f, 1.0f);
    }
}
