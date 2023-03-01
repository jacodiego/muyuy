#include "video.hpp"

namespace muyuy::video
{

    VideoEngine *videoManager = nullptr;

    const Color Color::White(1.0f, 1.0f, 1.0f);
    const Color Color::Gray(0.5f, 0.5f, 0.5f);
    const Color Color::Black(0.0f, 0.0f, 0.0f);
    const Color Color::Red(1.0f, 0.0f, 0.0f);
    const Color Color::Orange(1.0f, 0.4f, 0.0f);
    const Color Color::Yellow(1.0f, 1.0f, 0.0f);
    const Color Color::Green(0.0f, 1.0f, 0.0f);
    const Color Color::Blue(0.0f, 0.0f, 1.0f);

    VideoEngine::VideoEngine() : _fade_screen(nullptr)
    {
        window = SDL_CreateWindow("Muyuy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN);
    }

    void VideoEngine::initialize(SDL_Event *e)
    {
        event = e;
        device.initialize(window);
        renderer.initialize(event);

        // Fonts set
        _font_manager.addFont(FontTypes::UnboundedXs, "resources/fonts/Unbounded-Regular.ttf", 9);
        _font_manager.addFont(FontTypes::UnboundedSm, "resources/fonts/Unbounded-Regular.ttf", 12);
        _font_manager.addFont(FontTypes::UnboundedMd, "resources/fonts/Unbounded-Regular.ttf", 16);
        _font_manager.addFont(FontTypes::UnboundedLg, "resources/fonts/Unbounded-Regular.ttf", 24);
        _font_manager.addFont(FontTypes::UnboundedXl, "resources/fonts/Unbounded-Regular.ttf", 36);
        _font_manager.addFont(FontTypes::MontserratRegular, "resources/fonts/Montserrat-Regular.ttf", 36);

        // Fade Screen Init
        Texture *_overlay_fade = new Texture{device, &renderer};
        _overlay_fade->initialize(getWindowWidth(), getWindowHeight(), vk::Format::eR8G8B8A8Srgb, renderer.getDescriptorPool(poolTypes::Global), renderer.getDescriptorSetLayout(descriptorTypes::Sampler));
        uint8_t *fill = (uint8_t *)malloc(getWindowWidth() * getWindowHeight() * 4);
        memset(fill, 255, getWindowWidth() * getWindowHeight() * 4);
        _overlay_fade->addPixels(fill, 0, 0, getWindowWidth(), getWindowHeight());
        free(fill);
        _fade_screen = new FadeScreen{_overlay_fade};
    }

    void VideoEngine::startFrame()
    {
        renderer.startFrame();
    }

    void VideoEngine::endFrame()
    {
        renderer.endFrame();
    }

    void VideoEngine::write(Texture *texture, FontTypes type, std::string text)
    {
        _font_manager.write(texture, type, text);
    }

    void VideoEngine::swapBuffer()
    {
    }

    void VideoEngine::resize(int width, int height)
    {
        renderer.resize();
    }

    void VideoEngine::destroy()
    {
        device.waitIdle();
        SDL_DestroyWindow(window);
    }

    Texture *VideoEngine::createImage(const char *pathfile)
    {
        auto screen_find = _screen_textures.find(pathfile);
        if (screen_find == _screen_textures.end())
        {
            Texture *texture = new Texture{device, &renderer};
            texture->loadFromImage(pathfile, renderer.getDescriptorPool(poolTypes::Screen), renderer.getDescriptorSetLayout(descriptorTypes::Sampler));

            _screen_textures.insert(std::make_pair(pathfile, texture));
            return texture;
        }
        else
        {
            return screen_find->second;
        }
    }

    Texture *VideoEngine::createTexture()
    {
        Texture *texture = new Texture{device, &renderer};

        return texture;
    }

    void VideoEngine::update()
    {
        uint32_t frame_time = system::systemManager->getUpdateTime();
        _fade_screen->update(frame_time);
    }

    void VideoEngine::drawFade()
    {
        _fade_screen->draw();
    }

    void VideoEngine::fadeOut(uint32_t time)
    {
        _fade_screen->beginFadeOut(time);
    }

    void VideoEngine::fadeIn(uint32_t time)
    {
        _fade_screen->beginFadeIn(time);
    }

    void VideoEngine::addAnimation(const std::string &key, const std::string &script_file)
    {
        if (_animations.find(key) != _animations.end())
        {
            std::cerr << "Animation already exist" << std::endl;
            return;
        }
        _animations.insert(std::make_pair(key, new Animation{script_file}));
    }

    void VideoEngine::drawAnimation(const std::string &key, const std::string &desc, int x, int y)
    {
        if (_animations.find(key) == _animations.end())
        {
            std::cerr << "Animation not found" << std::endl;
            return;
        }
        _animations.at(key)->draw(desc, x, y);
    }

    void VideoEngine::clearScreenTextures()
    {
        device.waitIdle();
        for (auto texture : _screen_textures)
        {
            delete texture.second;
        }
        _screen_textures.clear();
        renderer.resetScreenDescriptorPool();
    }
}