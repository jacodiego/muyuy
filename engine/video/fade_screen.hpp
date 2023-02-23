#pragma once

#include "texture.hpp"

namespace muyuy::video
{

    class FadeScreen
    {
    public:
        FadeScreen(Texture *);
        void beginFadeIn(uint32_t);
        void beginFadeOut(uint32_t);
        void update(uint32_t);
        void draw();
        bool isFading() const
        {
            return _is_fading_in || _is_fading_out;
        }

    private:
        Texture *_overlay_texture;
        uint32_t _current_time;
        uint32_t _end_time;
        bool _is_fading_in;
        bool _is_fading_out;
        float _alpha;
    };

}
