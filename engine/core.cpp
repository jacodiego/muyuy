#include "core.hpp"

namespace muyuy
{

    using namespace boot;
    using namespace game;
    using namespace input;
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

        screenManager->push(new BootScreen(), false, false);

        bool isRunning = true;

        const uint32_t UPDATES_PER_SECOND = 70;
        const uint32_t SKIP_RENDER_TICKS = 1000 / UPDATES_PER_SECOND;
        uint32_t render_tick = SDL_GetTicks();
        uint32_t next_render_tick = 0;

        while (systemManager->isRunning())
        {
            // SDL_PollEvent(&event);

            // switch (event.type)
            // {
            // case SDL_QUIT:
            //     isRunning = false;
            //     break;
            // case SDL_WINDOWEVENT:
            //     if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            //         videoManager->resize(event.window.data1, event.window.data2);
            //     break;
            // default:
            //     break;
            // }

            render_tick = SDL_GetTicks();

            if (render_tick < next_render_tick)
            {
                SDL_Delay(next_render_tick - render_tick);
                continue;
            }

            systemManager->updateTimers(render_tick);
            inputManager->eventHandler();
            videoManager->update();
            screenManager->update();
            videoManager->startFrame();
            screenManager->draw();
            videoManager->drawFade();
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
        inputManager = InputEngine::singletonCreate();
        gameManager = GameEngine::singletonCreate();

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

        if (!inputManager->singletonInitialize())
        {
            throw std::runtime_error("ERROR: unable to initialize InputEngine");
        }

        if (!screenManager->singletonInitialize())
        {
            throw std::runtime_error("ERROR: unable to initialize ScreenEngine");
        }

        if (!loadSettings())
        {
            throw std::runtime_error("ERROR: unable to initialize Settings");
        }

        binding::bindEngine();
        binding::bindScreen();
        systemManager->initializeTimers();

        if (!gameManager->singletonInitialize())
        {
            throw std::runtime_error("ERROR: unable to initialize GameEngine");
        }
    }

    bool Core::loadSettings()
    {
        ScriptReader settings{"data/config/settings.lua"};

        if (!settings.openTable("settings"))
        {
            return false;
        }

        if (!settings.openTable("key_settings"))
        {
            return false;
        }

        int32_t key_code = settings.readInt("up");
        inputManager->setUpKey(static_cast<SDL_Keycode>(key_code));
        key_code = settings.readInt("down");
        inputManager->setDownKey(static_cast<SDL_Keycode>(key_code));
        key_code = settings.readInt("left");
        inputManager->setLeftKey(static_cast<SDL_Keycode>(key_code));
        key_code = settings.readInt("right");
        inputManager->setRightKey(static_cast<SDL_Keycode>(key_code));

        inputManager->setConfirmKey(static_cast<SDL_Keycode>(settings.readInt("confirm")));
        inputManager->setCancelKey(static_cast<SDL_Keycode>(settings.readInt("cancel")));
        inputManager->setMenuKey(static_cast<SDL_Keycode>(settings.readInt("menu")));
        inputManager->setMinimapKey(static_cast<SDL_Keycode>(settings.readInt("minimap")));
        inputManager->setPauseKey(static_cast<SDL_Keycode>(settings.readInt("pause")));
        settings.closeTable();

        return true;
    }

}
