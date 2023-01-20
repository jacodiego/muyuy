#pragma once

namespace muyuy::utils
{
    struct Position2D
    {
        Position2D() : x(0.0f),
                       y(0.0f)
        {
        }

        Position2D(float x_pos, float y_pos) : x(x_pos),
                                               y(y_pos)
        {
        }

        Position2D(const Position2D &other_pos)
        {
            // Prevents upon-self copy.
            if (&other_pos == this)
                return;

            x = other_pos.x;
            y = other_pos.y;
        }

        float GetLength2() const
        {
            return (x * x + y * y);
        }

        float GetDistance2(const Position2D &other_pos) const
        {
            float x2 = other_pos.x - x;
            x2 *= x2;
            float y2 = other_pos.y - y;
            y2 *= y2;
            return x2 + y2;
        }

        Position2D &operator=(const Position2D &other_pos)
        {
            if (&other_pos == this)
                return *this;

            x = other_pos.x;
            y = other_pos.y;

            return *this;
        }

        bool operator==(const Position2D &other_pos) const
        {
            if (&other_pos == this)
                return true;

            return x == other_pos.x && y == other_pos.y;
        }

        float x;
        float y;
    };

    typedef Position2D Vector2D;

    struct Line2D
    {
        Position2D begin;
        Position2D end;
    };

}
