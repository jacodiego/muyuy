#pragma once

#include "gui/option_menu.hpp"

#include "engine/input.hpp"
#include "engine/game.hpp"
#include "engine/screen.hpp"
#include "engine/script.hpp"
#include "engine/system.hpp"
#include "engine/video/video.hpp"
#include "engine/video/color.hpp"

namespace muyuy::boot
{
    enum class BootState
    {
        Intro,
        Credits,
        Menu
    };

    enum class MenuOptions
    {
        Start,
        Load,
        Options,
        Exit
    };

    class BootScreen : public screen::GameScreen
    {
    public:
        BootScreen();
        ~BootScreen();
        static BootScreen *currentInstance()
        {
            return _current_instance;
        }
        void reset();
        void update();
        void draw();
        BootState getState() const
        {
            return _boot_state;
        }
        void changeState(BootState boot_state)
        {
            _boot_state = boot_state;
        }

    private:
        BootState _boot_state;
        static BootScreen *_current_instance;
        bool _show_menu;
        bool _start_new_game;
        bool _exited;
        gui::OptionMenu _main_menu;
    };
};