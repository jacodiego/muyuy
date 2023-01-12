#pragma once

#include "device.hpp"
#include "utils/singleton.hpp"

#include <SDL2/SDL.h>

namespace muyuy
{
    namespace video
    {
        class VideoEngine;
        class Device;
        extern VideoEngine *videoManager;
        class VideoEngine : public utils::Singleton<VideoEngine>
        {
            friend class utils::Singleton<VideoEngine>;
            friend class Device;

        public:
            bool singletonInitialize() { return true; };
            void initialize();
            void destroy();

        private:
            VideoEngine();

        private:
            SDL_Window *window;
            Device *device;
        };

    }
}
