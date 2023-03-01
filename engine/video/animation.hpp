#pragma once

#include "video.hpp"
#include "texture.hpp"

#include "engine/system.hpp"

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <string>
#include <vector>
#include <unordered_map>

namespace muyuy::video
{
    struct AnimationFrame
    {
        uint8_t id;
        uint16_t duration;
    };
    class Animation
    {
    public:
        Animation(const std::string &);
        void draw(const std::string &, int, int);

    private:
        Texture *_texture;
        uint8_t _cols;
        uint8_t _rows;
        uint16_t _width;
        uint16_t _height;
        uint32_t _last_update;
        uint8_t _current_index;
        std::unordered_map<std::string, std::vector<AnimationFrame>> _frames;
        AnimationFrame *_current_frame;
    };
}