#pragma once

#include "utils/singleton.hpp"
#include "device.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "font_manager.hpp"
#include "texture_atlas.hpp"

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <iostream>
#include <map>
#include <vector>

namespace muyuy::video
{
    class VideoEngine;
    class Font;
    extern VideoEngine *videoManager;

    class VideoEngine : public utils::Singleton<VideoEngine>
    {
        friend class utils::Singleton<VideoEngine>;

    public:
        bool singletonInitialize() { return true; };
        void initialize(SDL_Event *);
        SDL_Event *getEvent() { return event; };
        void startFrame();
        void endFrame();
        void swapBuffer();
        void resize(int, int);
        Texture *createImage(const char *);
        void destroy();
        int getWindowWidth() { return renderer.getWindowExtent().width; };
        int getWindowHeight() { return renderer.getWindowExtent().height; };

    private:
        VideoEngine();

    private:
        SDL_Window *window;
        SDL_Event *event;
        Device device;
        Renderer renderer{device};
        std::vector<Texture *> _screen_textures;
        FontManager _font_manager;
        std::shared_ptr<Font> _font;
    };

}
