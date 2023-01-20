#include "video.hpp"

namespace muyuy::video
{

    VideoEngine *videoManager = nullptr;

    VideoEngine::VideoEngine()
    {
        window = SDL_CreateWindow("Muyuy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN);
    }

    void VideoEngine::initialize(SDL_Event *e)
    {
        event = e;
        device.initialize(window);
        renderer.initialize(event);
    }

    void VideoEngine::swapBuffer()
    {
    }

    void VideoEngine::resize(int width, int height)
    {
    }

    void VideoEngine::destroy()
    {
        SDL_DestroyWindow(window);
    }
}