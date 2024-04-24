#include "collition.hpp"

namespace muyuy::utils
{
    bool checkCollision(Rect a, Rect b)
    {
        // The sides of the rectangles
        int leftA, leftB;
        int rightA, rightB;
        int topA, topB;
        int bottomA, bottomB;

        // Calculate the sides of rect A
        leftA = a.x;
        rightA = a.x + a.width;
        topA = a.y;
        bottomA = a.y + a.height;

        // Calculate the sides of rect B
        leftB = b.x;
        rightB = b.x + b.width;
        topB = b.y;
        bottomB = b.y + b.height;

        // If any of the sides from A are outside of B
        if (bottomA <= topB)
        {
            return false;
        }

        if (topA >= bottomB)
        {
            return false;
        }

        if (rightA <= leftB)
        {
            return false;
        }

        if (leftA >= rightB)
        {
            return false;
        }

        // If none of the sides from A are outside B
        return true;
    }

}