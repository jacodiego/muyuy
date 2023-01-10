#pragma once

#include "engine/screen.hpp"
#include "engine/script.hpp"

namespace muyuy
{
    namespace boot
    {
        enum class BootState
        {
            Intro,
            Credits,
            Menu
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
            system::SystemTimer _boot_timer;
        };
    }
};