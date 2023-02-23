#include "fade_screen.hpp"

namespace muyuy::video
{

    FadeScreen::FadeScreen(Texture *texture) : _overlay_texture(texture),
                                               _is_fading_in(false),
                                               _is_fading_out(false),
                                               _current_time(0),
                                               _end_time(0),
                                               _alpha(0)
    {
        _overlay_texture->setColor(Color::Black);
    }

    void FadeScreen::beginFadeIn(uint32_t duration)
    {
        if (isFading())
            return;

        _is_fading_in = true;
        _end_time = duration;
        _current_time = 0;
        _alpha = 1;
        update(0);
    }

    void FadeScreen::beginFadeOut(uint32_t duration)
    {
        if (isFading())
            return;

        _is_fading_out = true;
        _end_time = duration;
        _current_time = 0;
        _alpha = 0;
        update(0);
    }
    void FadeScreen::update(uint32_t time)
    {
        if (!isFading())
            return;

        if (_current_time >= _end_time)
        {
            _is_fading_out = false;
            _is_fading_in = false;
            return;
        }

        if (_is_fading_in)
            _alpha = 1 - static_cast<float>(_current_time) / static_cast<float>(_end_time);
        else if (_is_fading_out)
            _alpha = static_cast<float>(_current_time) / static_cast<float>(_end_time);

        _current_time += time;
    }
    void FadeScreen::draw()
    {
        if (!isFading())
            return;
        _overlay_texture->draw(0, 0, 0, 0, _overlay_texture->getWidth(), _overlay_texture->getHeight(), _alpha, 1.0f, 1.0f);
    }
}