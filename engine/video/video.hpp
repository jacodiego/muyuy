#pragma once

#include "engine/system.hpp"
#include "utils/singleton.hpp"
#include "device.hpp"
#include "renderer.hpp"
#include "texture.hpp"
#include "texture_array.hpp"
#include "font_manager.hpp"
#include "fade_screen.hpp"
#include "animation.hpp"
#include "color.hpp"

#define GLM_FORCE_RADIANS 1
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <unordered_map>

namespace muyuy::video
{
    class VideoEngine;
    class Font;
    class Animation;
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
        Texture *createTexture();
        void destroy();
        void update();
        int getWindowWidth() { return renderer.getWindowExtent().width; };
        int getWindowHeight() { return renderer.getWindowExtent().height; };
        void write(Texture *, FontTypes, std::string);
        void fadeOut(uint32_t);
        void fadeIn(uint32_t);
        bool isFading() { return _fade_screen->isFading(); };
        void drawFade();
        void drawTextureArray(std::vector<RenderTile> tiles, const std::string &key) { renderer.drawTextureArray(tiles, _texture_arrays.at(key)); };
        void addAnimation(const std::string &, const std::string &);
        void drawAnimation(const std::string &key, const std::string &desc, int x, int y);
        void clearScreenTextures();
        void createTextureArray(const std::string &, std::vector<TextureMap>);

    private:
        VideoEngine();

    private:
        SDL_Window *window;
        SDL_Event *event;
        Device device;
        Renderer renderer{device};
        FontManager _font_manager{&renderer};
        FadeScreen *_fade_screen;
        std::unordered_map<std::string, Texture *> _screen_textures;
        std::unordered_map<std::string, TextureArray *> _texture_arrays;
        std::unordered_map<std::string, Animation *> _animations;
    };

}
