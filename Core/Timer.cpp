#include "Timer.hpp"

namespace Timer
{
    namespace
    {

        Uint64 mStartTicks;
        Uint64 mElapsedTicks;
        float mDeltaTime;
        float mTimeScale;
    }

    void init()
    {
        reset();
        mTimeScale = 1.0f;
    }

    void reset()
    {
        mStartTicks = SDL_GetTicks64();
        mElapsedTicks = 0;
        mDeltaTime = 0.0f;
    }

    float deltaTime()
    {
        return mDeltaTime;
    }

    void setTimeScale(float t)
    {
        mTimeScale = t;
    }

    float getTimeScale()
    {
        return mTimeScale;
    }

    void update()
    {
        mElapsedTicks = SDL_GetTicks64() - mStartTicks;
        mDeltaTime = mElapsedTicks * 0.001f;
    }
}