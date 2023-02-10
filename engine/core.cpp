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

        const uint32_t UPDATES_PER_SECOND = 60 + 10;
        const uint32_t SKIP_RENDER_TICKS = 1000 / UPDATES_PER_SECOND;
        uint32_t render_tick = SDL_GetTicks();
        uint32_t next_render_tick = 0;

        while (isRunning)
        {
            SDL_PollEvent(&event);

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

            render_tick = SDL_GetTicks();

            if (render_tick < next_render_tick)
            {
                SDL_Delay(next_render_tick - render_tick);
                continue;
            }

            systemManager->updateTimers(render_tick);
            screenManager->update();
            videoManager->startFrame();
            screenManager->draw();
            videoManager->endFrame();

            videoManager->swapBuffer();
            next_render_tick = SDL_GetTicks() + SKIP_RENDER_TICKS;
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
