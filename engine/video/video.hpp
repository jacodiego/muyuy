#pragma once

#include "utils/singleton.hpp"
#include "device.hpp"
#include "renderer.hpp"

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <iostream>

namespace muyuy::video
{
    class VideoEngine;
    extern VideoEngine *videoManager;
    class VideoEngine : public utils::Singleton<VideoEngine>
    {
        friend class utils::Singleton<VideoEngine>;

    public:
        bool singletonInitialize() { return true; };
        void initialize(SDL_Event *);
        SDL_Event *getEvent() { return event; };
        void draw();
        void swapBuffer();
        void resize(int width, int height);
        void destroy();

    private:
        VideoEngine();

    private:
        SDL_Window *window;
        SDL_Event *event;
        Device device;
        Renderer renderer{device};
    };

}
