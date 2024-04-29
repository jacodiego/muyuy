#pragma once

#include "utils/singleton.hpp"

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_events.h>

namespace muyuy::input
{

    class InputEngine;
    extern InputEngine *inputManager;

    class KeyState
    {
    public:
        SDL_Keycode up;
        SDL_Keycode down;
        SDL_Keycode left;
        SDL_Keycode right;
        SDL_Keycode run;
        SDL_Keycode confirm;
        SDL_Keycode cancel;
        SDL_Keycode menu;
        SDL_Keycode minimap;
        SDL_Keycode pause;
    };

    class InputEngine : public utils::Singleton<InputEngine>
    {
        friend class utils::Singleton<InputEngine>;

    public:
        ~InputEngine();

        bool singletonInitialize() { return true; };
        void eventHandler();
        bool isQuitPress() const
        {
            return _quit_press;
        }

        bool anyRegisteredKeyPress() const
        {
            return _registered_key_press;
        }

        bool upPress() const
        {
            return _up_press;
        }

        bool downPress() const
        {
            return _down_press;
        }

        bool leftPress() const
        {
            return _left_press;
        }

        bool rightPress() const
        {
            return _right_press;
        }
        bool confirmPress() const
        {
            return _confirm_press;
        }
        bool runPress() const
        {
            return _run_press;
        }

        void setUpKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.up, key);
        }

        void setDownKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.down, key);
        }

        void setLeftKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.left, key);
        }

        void setRightKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.right, key);
        }

        void setRunKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.run, key);
        }

        void setConfirmKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.confirm, key);
        }

        void setCancelKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.cancel, key);
        }

        void setMenuKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.menu, key);
        }

        void setMinimapKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.minimap, key);
        }

        void setPauseKey(const SDL_Keycode &key)
        {
            _setNewKey(_key.pause, key);
        }

        bool upState() const
        {
            return _up_state;
        }

        bool downState() const
        {
            return _down_state;
        }

        bool leftState() const
        {
            return _left_state;
        }

        bool rightState() const
        {
            return _right_state;
        }

        bool confirmState() const
        {
            return _confirm_state;
        }

        bool runState() const
        {
            return _run_state;
        }

        bool cancelState() const
        {
            return _cancel_state;
        }

        bool menuState() const
        {
            return _menu_state;
        }

    private:
        InputEngine();
        void _keyEventHandler(SDL_KeyboardEvent &key_event);
        void _setNewKey(SDL_Keycode &old_key, SDL_Keycode new_key);

    private:
        SDL_Event _key_event;
        KeyState _key;
        bool _registered_key_press;
        bool _registered_key_release;
        bool _up_state;
        bool _down_state;
        bool _left_state;
        bool _right_state;
        bool _run_state;
        bool _confirm_state;
        bool _cancel_state;
        bool _menu_state;
        bool _up_press;
        bool _down_press;
        bool _left_press;
        bool _right_press;
        bool _run_press;
        bool _confirm_press;
        bool _cancel_press;
        bool _menu_press;
        bool _minimap_press;
        bool _pause_press;
        bool _quit_press;
        bool _help_press;
        bool _up_release;
        bool _down_release;
        bool _left_release;
        bool _right_release;
        bool _run_release;
        bool _confirm_release;
        bool _cancel_release;
        bool _menu_release;
        bool _minimap_release;
        bool _pause_release;
        bool _quit_release;
        bool _help_release;
    };

}