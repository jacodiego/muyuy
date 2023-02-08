#include "boot.hpp"

#include <iostream>

namespace muyuy
{
    namespace boot
    {
        BootScreen *BootScreen::_current_instance = nullptr;

        BootScreen::BootScreen() : GameScreen(screen::ScreenType::Boot),
                                   _boot_state(BootState::Intro)
        {
            _current_instance = this;

            getScriptSupervisor().addScript("data/boot/boot.lua");
            getScriptSupervisor().initialize(this);
        }

        BootScreen::~BootScreen() {}

        void BootScreen::reset()
        {
        }

        void BootScreen::update()
        {
            GameScreen::update();
            if (_boot_state == BootState::Intro)
            {
                // Press any key to menu
            }

            if (_boot_state == BootState::Menu)
            {
                _show_menu = true;
            }
        }

        void BootScreen::draw()
        {
            getScriptSupervisor().draw();
            video::videoManager->draw();
        }
    }
};