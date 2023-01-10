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

            _boot_timer.initialize(14000);
            _boot_timer.enableManualUpdate();
            _boot_timer.run();
        }

        BootScreen::~BootScreen() {}

        void BootScreen::reset()
        {
            _current_instance = this;

            // Set the coordinate system that BootMode uses
            // VideoManager->SetStandardCoordSys();
            // VideoManager->SetDrawFlags(VIDEO_X_CENTER, VIDEO_Y_CENTER, 0);

            // GlobalManager->ClearAllData(); // Resets the game universe to a nullptr state

            // getScriptSupervisor().reset();
        }

        void BootScreen::update()
        {
            // Update the game mode generic members.
            GameScreen::update();

            // if(_exiting_to_new_game) {
            //     // When the fade out is done, we start a new game.
            //     if (!VideoManager->IsFading() && _new_game_called == false) {
            //         _new_game_called = true;
            //         if (!GlobalManager->NewGame()) {
            //             _new_game_called = false;
            //             _exiting_to_new_game = false;
            //             VideoManager->FadeIn(2000);
            //             ChangeState(BOOT_STATE_MENU);
            //         }
            //     }
            //     return;
            // }

            if (_boot_state == BootState::Intro)
            {
                // if (InputManager->AnyRegisteredKeyPress())
                // {
                //     ChangeState(BOOT_STATE_MENU);
                //     return;
                // }
                // else
                // {
                //     return; // Otherwise skip rest of the event handling for now
                // }
            }

            // Updates the main menu when the option menu handler isn't active.
            // bool is_menu_active = _menu_handler.IsActive();
            // if (is_menu_active)
            // {
            //     bool was_showing_first_run_dlg =
            //         _menu_handler.IsShowingFirstRunLanguageMenu();
            //     _menu_handler.Update();

            //     // We do this to prevent unwanted input going to the main menu
            //     // after the first app run sequence.
            //     if (was_showing_first_run_dlg)
            //         return;
            // }
            // else
            // {
            //     _main_menu.Update();
            // }

            // Update also the bar and f1 help text alpha
            uint32_t time_expired = system::systemManager->getUpdateTime();
            _boot_timer.update(time_expired);
            // if (_boot_timer.GetTimeExpired() >= 4000.0 &&
            //     _boot_timer.GetTimeExpired() < 12000.0)
            // {
            //     _help_text_alpha += 0.001f * time_expired;
            //     if (_help_text_alpha > 1.0f)
            //         _help_text_alpha = 1.0f;
            // }
            // else if (_boot_timer.GetTimeExpired() >= 12000.0 &&
            //          _boot_timer.GetTimeExpired() < 14000.0)
            // {
            //     _help_text_alpha -= 0.001f * time_expired;
            //     if (_help_text_alpha < 0.0f)
            //         _help_text_alpha = 0.0f;
            // }

            // if (_menu_bar_alpha < 0.6f)
            // {
            //     _menu_bar_alpha = _menu_bar_alpha + 0.001f * time_expired;
            //     if (_menu_bar_alpha >= 0.6f)
            //         _menu_bar_alpha = 0.6f;
            // }

            // if (is_menu_active)
            //     return;

            // // Handles the main menu input.

            // // Only quit when we are at the main menu level.
            // if (InputManager->QuitPress())
            // {
            //     // Don't quit the game when using the joystick,
            //     // as it is confusing for the user.
            //     SDL_Event ev = InputManager->GetMostRecentKeyEvent();
            //     if (ev.type == SDL_KEYDOWN)
            //         SystemManager->ExitGame();
            // }

            // if (InputManager->LeftPress())
            // {
            //     GlobalManager->Media().PlaySound("bump");
            //     _main_menu.InputLeft();
            //     return;
            // }
            // else if (InputManager->RightPress())
            // {
            //     GlobalManager->Media().PlaySound("bump");
            //     _main_menu.InputRight();
            //     return;
            // }
            // else if (!InputManager->ConfirmPress())
            //     return;

            // // Confirm press
            // int32_t selection = _main_menu.GetSelection();
            // switch (selection)
            // {
            // default:
            //     break;
            // case 0: // New Game
            //     _OnNewGame();
            //     break;
            // case 1:
            //     _OnLoadGame();
            //     break;
            // case 2:
            //     _OnOptions();
            //     break;
            // case 3:
            //     _OnQuit();
            //     break;
            // }
        }

        void BootScreen::draw()
        {
            // VideoManager->PushState();
            // VideoManager->SetDrawFlags(VIDEO_X_LEFT, VIDEO_Y_TOP, VIDEO_BLEND, 0);
            // VideoManager->SetStandardCoordSys();

            // GetScriptSupervisor().DrawBackground();
            // GetScriptSupervisor().DrawForeground();
            // VideoManager->PopState();
        }
    }
};