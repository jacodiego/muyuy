#pragma once

#include <SDL2/SDL.h>

namespace Timer
{
    void init();
    void reset();
    float deltaTime();
    void setTimeScale(float t);
    float getTimeScale();
    void update();
};