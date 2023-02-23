#include "boot.hpp"

#include <iostream>

namespace muyuy::boot
{
    BootScreen *BootScreen::_current_instance = nullptr;

    BootScreen::BootScreen() : GameScreen(screen::ScreenType::Boot),
                               _boot_state(BootState::Intro),
                               _start_new_game(false)
    {
        _current_instance = this;

        getScriptSupervisor().addScript("data/boot/boot.lua");
        getScriptSupervisor().initialize(this);
        _main_menu.setPosition(130, 500);
        _main_menu.setFont(video::FontTypes::UnboundedMd);
        _main_menu.setColors(video::Color::White, video::Color::Orange);
        _main_menu.addOption("Comenzar");
        _main_menu.addOption("Cargar");
        _main_menu.addOption("Opciones");
        _main_menu.addOption("Salir");
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
            if (input::inputManager->anyRegisteredKeyPress())
            {
                changeState(BootState::Menu);
                return;
            }
        }

        if (_boot_state == BootState::Menu)
        {
            if (_start_new_game)
            {
                if (!video::videoManager->isFading())
                {
                    _start_new_game = false;
                    game::gameManager->newGame();
                }
                return;
            }

            _main_menu.update();
            if (input::inputManager->rightPress())
            {
                _main_menu.moveRight();
            }
            if (input::inputManager->leftPress())
            {
                _main_menu.moveLeft();
            }
            if (input::inputManager->confirmPress())
            {
                switch (_main_menu.getSelected())
                {
                case 0:
                    _start_new_game = true;
                    video::videoManager->fadeOut(2000);
                    break;
                case 3:
                    system::systemManager->exitGame();
                    break;
                default:
                    break;
                }
            }
        }
    }

    void BootScreen::draw()
    {
        getScriptSupervisor().draw();
        if (_boot_state == BootState::Menu)
        {
            _main_menu.draw();
        }
    }
};