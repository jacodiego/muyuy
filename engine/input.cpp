#include "input.hpp"

namespace muyuy::input
{

    InputEngine *inputManager = nullptr;

    InputEngine::InputEngine()
    {

        _registered_key_press = false;
        _registered_key_release = false;

        // _last_axis_moved      = -1;
        _up_state = false;
        _up_press = false;
        _up_release = false;
        _down_state = false;
        _down_press = false;
        _down_release = false;
        _left_state = false;
        _left_press = false;
        _left_release = false;
        _right_state = false;
        _right_press = false;
        _right_release = false;
        _confirm_state = false;
        _confirm_press = false;
        _confirm_release = false;
        _cancel_state = false;
        _cancel_press = false;
        _cancel_release = false;
        _menu_state = false;
        _menu_press = false;
        _menu_release = false;
        _minimap_press = false;
        _minimap_release = false;

        _pause_press = false;
        _quit_press = false;
        _help_press = false;

        memset(&_key, 0, sizeof(_key));
    }

    InputEngine::~InputEngine()
    {
    }

    void InputEngine::eventHandler()
    {
        SDL_Event event;

        _registered_key_press = false;
        _registered_key_release = false;

        _up_press = false;
        _up_release = false;
        _down_press = false;
        _down_release = false;
        _left_press = false;
        _left_release = false;
        _right_press = false;
        _right_release = false;
        _confirm_press = false;
        _confirm_release = false;
        _cancel_press = false;
        _cancel_release = false;
        _menu_press = false;
        _menu_release = false;
        _minimap_press = false;
        _minimap_release = false;

        _pause_press = false;
        _pause_release = false;
        _quit_press = false;
        _quit_release = false;
        _help_press = false;
        _help_release = false;

        // Loops until there are no remaining events to process
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                _key_event = event;
                _quit_press = true;
                break;
            }
            else if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN)
            {
                _key_event = event;
                _keyEventHandler(event.key);
            }
        }

        _registered_key_press = _up_press || _down_press || _left_press || _right_press || _quit_press ||
                                _confirm_press || _cancel_press || _minimap_press || _menu_press || _pause_press ||
                                _help_press;

        _registered_key_release = _up_release || _down_release || _left_release || _right_release || _quit_release ||
                                  _confirm_release || _cancel_release || _minimap_release || _menu_release || _pause_release ||
                                  _help_release;
    }

    void InputEngine::_keyEventHandler(SDL_KeyboardEvent &key_event)
    {
        if (key_event.type == SDL_KEYDOWN)
        {
            //_any_keyboard_key_press = true;

            if (key_event.keysym.mod & KMOD_CTRL || key_event.keysym.sym == SDLK_LCTRL || key_event.keysym.sym == SDLK_RCTRL)
            { // CTRL key was held down
                if (key_event.keysym.sym == SDLK_f)
                {
                    // Toggle between full-screen and windowed mode
                    // VideoManager->ToggleFullscreen();
                    // VideoManager->ApplySettings();
                    return;
                }
                else if (key_event.keysym.sym == SDLK_q)
                {
                    _quit_press = true;
                }
                else if (key_event.keysym.sym == SDLK_s)
                {
                    // Take a screenshot of the current game
                    // static uint32_t i = 1;
                    // std::string path = "";
                    // while (true)
                    // {
                    //     path = GetUserDataPath() + "screenshot_" + NumberToString<uint32_t>(i) + ".png";
                    //     if (!DoesFileExist(path))
                    //         break;
                    //     i++;
                    // }
                    // VideoManager->MakeScreenshot(path);
                    return;
                }
            } // endif CTRL pressed

            // Note: a switch-case statement won't work here because Key.up is not an
            // integer value the compiler will whine and cry about it ;_;
            if (key_event.keysym.sym == SDLK_ESCAPE)
            {
                // Hide the help window if shown
                // HelpWindow *help_window = ModeManager->GetHelpWindow();
                // if (help_window && help_window->IsActive())
                // {
                //     help_window->Hide();
                //     return;
                // }

                // Handle the normal events otherwise.
                _quit_press = true;
                return;
            }
            else if (key_event.keysym.sym == _key.up)
            {
                _up_state = true;
                _up_press = true;
                return;
            }
            else if (key_event.keysym.sym == _key.down)
            {
                _down_state = true;
                _down_press = true;
                return;
            }
            else if (key_event.keysym.sym == _key.left)
            {
                _left_state = true;
                _left_press = true;
                return;
            }
            else if (key_event.keysym.sym == _key.right)
            {
                _right_state = true;
                _right_press = true;
                return;
            }
            else if (key_event.keysym.sym == _key.confirm || key_event.keysym.sym == SDLK_RETURN || key_event.keysym.sym == SDLK_KP_ENTER)
            {
                _confirm_state = true;
                _confirm_press = true;
                return;
            }
            else if (key_event.keysym.sym == _key.cancel)
            {
                _cancel_state = true;
                _cancel_press = true;
                return;
            }
            else if (key_event.keysym.sym == _key.menu)
            {
                _menu_state = true;
                _menu_press = true;
                return;
            }
            else if (key_event.keysym.sym == _key.minimap)
            {
                _minimap_press = true;
                return;
            }
            else if (key_event.keysym.sym == _key.pause)
            {
                _pause_press = true;
                return;
            }
            else if (key_event.keysym.sym == SDLK_F1)
            {
                _help_press = true;
                // Toggle the help window visibility
                // HelpWindow *help_window = ModeManager->GetHelpWindow();
                // if (!help_window)
                //     return;
                // if (!help_window->IsActive())
                //     help_window->Show();
                // else
                //     help_window->Hide();
                return;
            }
        }
        else
        { // Key was released

            if (key_event.keysym.sym == _key.up)
            {
                _up_state = false;
                _up_release = true;
                return;
            }
            else if (key_event.keysym.sym == _key.down)
            {
                _down_state = false;
                _down_release = true;
                return;
            }
            else if (key_event.keysym.sym == _key.left)
            {
                _left_state = false;
                _left_release = true;
                return;
            }
            else if (key_event.keysym.sym == _key.right)
            {
                _right_state = false;
                _right_release = true;
                return;
            }
            else if (key_event.keysym.sym == _key.confirm || key_event.keysym.sym == SDLK_RETURN || key_event.keysym.sym == SDLK_KP_ENTER)
            {
                _confirm_state = false;
                _confirm_release = true;
                return;
            }
            else if (key_event.keysym.sym == _key.cancel)
            {
                _cancel_state = false;
                _cancel_release = true;
                return;
            }
            else if (key_event.keysym.sym == _key.menu)
            {
                _menu_state = false;
                _menu_release = true;
                return;
            }
            else if (key_event.keysym.sym == _key.minimap)
            {
                _minimap_release = true;
                return;
            }
            else if (key_event.keysym.sym == _key.pause)
            {
                _pause_release = true;
                return;
            }
            else if (key_event.keysym.sym == SDLK_F1)
            {
                _help_release = true;
                return;
            }
            else if (key_event.keysym.sym == SDLK_ESCAPE)
            {
                _quit_release = true;
                return;
            }
        }
    }

    void InputEngine::_setNewKey(SDL_Keycode &old_key, SDL_Keycode new_key)
    {
        if (new_key == SDLK_ESCAPE || new_key == SDLK_F1 ||
            new_key == SDLK_RETURN || new_key == SDLK_KP_ENTER)
            return;

        if (_key.up == new_key)
        {
            _key.up = old_key;
            old_key = new_key;
            return;
        }
        if (_key.down == new_key)
        {
            _key.down = old_key;
            old_key = new_key;
            return;
        }
        if (_key.left == new_key)
        { // left key used already
            _key.left = old_key;
            old_key = new_key;
            return;
        }
        if (_key.right == new_key)
        {
            _key.right = old_key;
            old_key = new_key;
            return;
        }
        if (_key.confirm == new_key)
        {
            _key.confirm = old_key;
            old_key = new_key;
            return;
        }
        if (_key.cancel == new_key)
        {
            _key.cancel = old_key;
            old_key = new_key;
            return;
        }
        if (_key.menu == new_key)
        {
            _key.menu = old_key;
            old_key = new_key;
            return;
        }
        if (_key.minimap == new_key)
        {
            _key.minimap = old_key;
            old_key = new_key;
            return;
        }
        if (_key.pause == new_key)
        {
            _key.pause = old_key;
            old_key = new_key;
            return;
        }

        old_key = new_key;
    }

}