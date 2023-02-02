#include "core.hpp"

namespace muyuy
{

    using namespace boot;
    using namespace screen;
    using namespace script;
    using namespace system;
    using namespace video;

    namespace binding
    {
        void bindEngine();
        void bindScreen();
    }

    void Core::run()
    {
        SDL_Init(SDL_INIT_EVERYTHING);

        initializeEngine();

        videoManager->initialize(&event);

        screenManager->push(new BootScreen(), false, true);

        bool isRunning = true;

        while (isRunning)
        {
            SDL_PollEvent(&event);

            screenManager->update();
            screenManager->draw();

            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    videoManager->resize(event.window.data1, event.window.data2);
                break;
            default:
                break;
            }

            videoManager->swapBuffer();
        }
        videoManager->destroy();
        SDL_Quit();
    }

    void Core::initializeEngine()
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
