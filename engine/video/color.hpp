#pragma once

#include <glm/glm.hpp>

namespace muyuy::video
{
    class Color
    {
    public:
        Color(float r, float b, float g)
        {
            _color = {r, b, g};
        };

        glm::vec3 getVec3() { return _color; };

    public:
        static const Color White;
        static const Color Gray;
        static const Color Black;
        static const Color Red;
        static const Color Orange;
        static const Color Yellow;
        static const Color Green;
        static const Color Blue;

    private:
        glm::vec3 _color;
    };
}