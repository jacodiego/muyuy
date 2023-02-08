#include "video.hpp"

namespace muyuy::video
{

    VideoEngine *videoManager = nullptr;

    VideoEngine::VideoEngine() : _font_manager(nullptr)
    {
        window = SDL_CreateWindow("Muyuy", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    }

    void VideoEngine::initialize(SDL_Event *e)
    {
        event = e;
        device.initialize(window);
        renderer.initialize(event);

        auto font_atlas = std::make_shared<TextureAtlas>();
        font_atlas->initialize(&renderer, 512, 512);
        _font_manager = FontManager(font_atlas);
        _font = _font_manager.getFont("resources/fonts/Unbounded-Regular.ttf", 36);
    }

    void VideoEngine::draw()
    {
        renderer.draw();
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
        Texture *texture = new Texture{device, &renderer};
        texture->load(pathfile, renderer.getDescriptorPool(descriptorTypes::Sampler), renderer.getDescriptorSetLayout(descriptorTypes::Sampler));

        _screen_textures.push_back(texture);

        return texture;
    }
}