#include "video.hpp"

namespace muyuy
{
    namespace video
    {

        VideoEngine *videoManager = nullptr;

        VideoEngine::VideoEngine()
        {
            window = SDL_CreateWindow("Muyuy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
        }

        void VideoEngine::initialize()
        {
            device = new Device{};
        }

        void VideoEngine::destroy()
        {

            SDL_DestroyWindow(window);
        }
    }
}