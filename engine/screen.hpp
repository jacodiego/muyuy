#pragma once

#include "engine/script_supervisor.hpp"
#include "engine/system.hpp"
#include "utils/singleton.hpp"

namespace muyuy::screen
{

    using namespace script;

    class ScreenEngine;
    extern ScreenEngine *screenManager;

    enum class ScreenType
    {
        Dummy,
        Boot,
        Map,
        Battle,
        WorldMap,
        MainMenu
    };

    class GameScreen
    {
        friend class ScreenEngine;

    public:
        GameScreen();
        explicit GameScreen(ScreenType st);
        virtual ~GameScreen();
        ScreenType getScreenType() const
        {
            return _screen_type;
        }
        virtual void update();
        virtual void draw() = 0;
        virtual void deactivate();
        virtual void reset() = 0;
        ScriptSupervisor &getScriptSupervisor();

    protected:
        ScreenType _screen_type;

    private:
        ScriptSupervisor _script_supervisor;
    };

    class ScreenEngine : public utils::Singleton<ScreenEngine>
    {
        friend class utils::Singleton<ScreenEngine>;

    public:
        ~ScreenEngine();

        bool singletonInitialize();

        void pop(bool fade_out = false, bool fade_in = false);
        // void popAll();
        void push(GameScreen *gs, bool fade_out = false, bool fade_in = false);
        GameScreen *getTop();
        GameScreen *get(uint32_t index);
        void update();
        void draw();
        ScreenType getScreenType() const;
        ScreenType getScreenType(uint32_t index) const;

    private:
        ScreenEngine();
        std::vector<GameScreen *> _game_stack;
        std::vector<GameScreen *> _push_stack;
        bool _state_change;
        uint32_t _pop_count;
        bool _fade_in;
        bool _fade_out;
        bool _fade_out_finished;
    };

}