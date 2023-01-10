#include <SDL2/SDL.h>
#include "engine/screen.hpp"
#include "engine/script.hpp"
#include "engine/system.hpp"
#include "engine/video/video.hpp"

#include "screens/boot/boot.hpp"

using namespace muyuy::boot;
using namespace muyuy::screen;
using namespace muyuy::script;
using namespace muyuy::system;
using namespace muyuy::video;

namespace muyuy
{

    namespace binding
    {
        void bindEngine();
        void bindScreen();
    }

    static void initializeEngine()
    {

        scriptManager = ScriptEngine::singletonCreate();
        videoManager = VideoEngine::singletonCreate();
        systemManager = SystemEngine::singletonCreate();
        screenManager = ScreenEngine::singletonCreate();

        if (!scriptManager->singletonInitialize())
        {
            throw std::runtime_error("ERROR: unable to initialize ScriptEngine");
        }

        if (!videoManager->singletonInitialize())
        {
            throw std::runtime_error("ERROR: unable to initialize VideoEngine");
        }

        if (!systemManager->singletonInitialize())
        {
            throw std::runtime_error("ERROR: unable to initialize SystemEngine");
        }

        if (!screenManager->singletonInitialize())
        {
            throw std::runtime_error("ERROR: unable to initialize ScreenEngine");
        }

        binding::bindEngine();
        binding::bindScreen();
        systemManager->initializeTimers();
    }
}

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Muyui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    SDL_Event event;

    muyuy::initializeEngine();

    screenManager->push(new BootScreen(), false, true);

    bool isRunning = true;

    videoManager->initVulkan(window, &event);

    while (isRunning)
    {
        SDL_PollEvent(&event);

        videoManager->drawFrame();
        screenManager->update();

        switch (event.type)
        {
        case SDL_QUIT:
            isRunning = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                videoManager->resizeScreen(event.window.data1, event.window.data2);
            break;
        default:
            break;
        }
    }
    videoManager->waitIdle();
    videoManager->cleanup();
    SDL_DestroyWindow(window);
    SDL_Quit();
}