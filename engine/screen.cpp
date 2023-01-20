#include "screen.hpp"

namespace muyuy::screen
{

    ScreenEngine *screenManager = nullptr;
    const uint32_t FADE_IN_OUT_TIME = 700;

    //***********************************************************
    //********* GameScreen
    //***********************************************************

    GameScreen::GameScreen() : _screen_type(ScreenType::Dummy)
    {
    }

    GameScreen::GameScreen(ScreenType st)
    {
        _screen_type = st;
    }

    GameScreen::~GameScreen() {}

    void GameScreen::update()
    {
        uint32_t frame_time = system::systemManager->getUpdateTime();

        _script_supervisor.update();
        //_effect_supervisor.Update(frame_time);
    }

    void GameScreen::deactivate()
    {
    }

    ScriptSupervisor &GameScreen::getScriptSupervisor()
    {
        return _script_supervisor;
    }

    //***********************************************************
    //********* ScreenEngine
    //***********************************************************

    ScreenEngine::ScreenEngine() : _fade_out(false), _fade_in(false)
    {
    }

    ScreenEngine::~ScreenEngine()
    {
        while (!_game_stack.empty())
        {
            delete _game_stack.back();
            _game_stack.pop_back();
        }
    }
    bool ScreenEngine::singletonInitialize()
    {
        while (!_game_stack.empty())
        {
            delete _game_stack.back();
            _game_stack.pop_back();
        }

        return true;
    }

    void ScreenEngine::pop(bool fade_out, bool fade_in)
    {
        _fade_in = fade_in;

        if (fade_out)
        {
            // Start a fade out between the two game modes.
            // VideoManager->_StartTransitionFadeOut(Color::black, FADE_IN_OUT_TIME);
            _fade_out = true;
            _fade_out_finished = false;
        }
        else
        {
            _fade_out = false;
            _fade_out_finished = true;
        }
    }

    void ScreenEngine::push(GameScreen *gs, bool fade_out, bool fade_in)
    {
        _push_stack.push_back(gs);
        _state_change = true;
        _fade_in = fade_in;
        if (fade_out)
        {
            // VideoManager->_StartTransitionFadeOut(Color::black, FADE_IN_OUT_TIME);
            _fade_out = true;
            _fade_out_finished = false;
        }
        else
        {
            _fade_out = false;
            _fade_out_finished = true;
        }
    }

    ScreenType ScreenEngine::getScreenType() const
    {
        if (_game_stack.empty())
            return ScreenType::Dummy;
        return _game_stack.back()->getScreenType();
    }

    ScreenType ScreenEngine::getScreenType(uint32_t index) const
    {
        if (_game_stack.size() < index)
            return ScreenType::Dummy;
        return _game_stack.at(_game_stack.size() - index)->getScreenType();
    }

    GameScreen *ScreenEngine::getTop()
    {
        if (_game_stack.empty())
            return nullptr;
        return _game_stack.back();
    }

    GameScreen *ScreenEngine::get(uint32_t index)
    {
        GameScreen *result = nullptr;

        assert(_game_stack.size() > index);
        if (_game_stack.size() > index)
        {
            result = _game_stack.at(_game_stack.size() - index - 1);
        }

        return result;
    }

    void ScreenEngine::update()
    {
        // Check whether the fade out is done.
        // if (_fade_out && VideoManager->IsLastFadeTransitional() &&
        //     !VideoManager->IsFading())
        // {
        //     _fade_out = false;
        //     _fade_out_finished = true;
        // }

        if (_fade_out_finished && _state_change)
        {
            while (_pop_count != 0)
            {
                if (_game_stack.empty())
                {
                    _pop_count = 0;
                    break;
                }
                delete _game_stack.back();
                _game_stack.pop_back();
                _pop_count--;
            }

            while (!_push_stack.empty())
            {
                if (!_game_stack.empty() && _game_stack.back())
                    _game_stack.back()->deactivate();

                _game_stack.push_back(_push_stack.back());
                _push_stack.pop_back();
            }

            // Make sure there is a game mode on the stack,
            // otherwise we'll get a segmentation fault.
            // if (_game_stack.empty())
            // {
            //     PRINT_WARNING << "game stack is empty, exiting application" << std::endl;
            //     SystemManager->ExitGame();
            // }

            _game_stack.back()->reset();
            _state_change = false;
            _fade_out_finished = false;

            // // We can now fade in, or not
            // VideoManager->_TransitionalFadeIn(_fade_in ? FADE_IN_OUT_TIME : 0);

            // // Call the system manager and tell it that the active game mode changed
            // // so it can update timers accordingly
            // SystemManager->ExamineSystemTimers();

            // // Re-initialize the game update timer so that the new active game mode does not begin with any update time to process
            // SystemManager->InitializeUpdateTimer();
        }

        if (!_game_stack.empty())
            _game_stack.back()->update();
    }

    void ScreenEngine::draw()
    {
        if (_game_stack.empty())
            return;

        _game_stack.back()->draw();
    }
}