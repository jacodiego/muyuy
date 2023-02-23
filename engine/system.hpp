#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <SDL2/SDL.h>
#include <map>
#include <set>
#include "engine/input.hpp"
#include "engine/screen.hpp"
#include "utils/singleton.hpp"

namespace muyuy::screen
{
    class GameScreen;
}
namespace muyuy::system
{
    class SystemEngine;
    extern SystemEngine *systemManager;

    enum class TimerState
    {
        Created,
        Initialized,
        Running,
        Paused,
        Finished
    };

    class SystemTimer
    {
        friend class SystemEngine;

    public:
        SystemTimer();
        SystemTimer(uint32_t duration, int32_t loops = 0);
        ~SystemTimer();
        void initialize(uint32_t duration, int32_t loops = 0);
        void enableAutoUpdate(screen::GameScreen *owner = nullptr);
        void enableManualUpdate();
        virtual void update();
        virtual void update(uint32_t time);
        virtual void reset()
        {
            if (_state != TimerState::Created)
            {
                _state = TimerState::Initialized;
                _time_expired = 0;
                _times_completed = 0;
            }
        }

        void run()
        {
            if (isInitialized() || isPaused())
                _state = TimerState::Running;
        }

        void pause()
        {
            if (isRunning())
                _state = TimerState::Paused;
        }

        void finish()
        {
            _state = TimerState::Finished;
        }

        bool isInitialized() const
        {
            return (_state == TimerState::Initialized);
        }

        bool isRunning() const
        {
            return (_state == TimerState::Running);
        }

        bool isPaused() const
        {
            return (_state == TimerState::Paused);
        }

        bool isFinished() const
        {
            return (_state == TimerState::Finished);
        }
        bool isAutoUpdate() const
        {
            return _auto_update;
        }
        uint32_t getTimeXpirated() { return _time_expired; };
        screen::GameScreen *getScreenOwner() const
        {
            return _screen_owner;
        }

    protected:
        TimerState _state;
        bool _auto_update;
        uint32_t _duration;
        int32_t _number_loops;
        uint32_t _time_expired;
        uint32_t _times_completed;
        screen::GameScreen *_screen_owner;
        virtual void _autoUpdate();
        void _updateTimer(uint32_t amount);
    };

    class SystemEngine : public utils::Singleton<SystemEngine>
    {
        friend class utils::Singleton<SystemEngine>;

    public:
        bool singletonInitialize();
        void initializeTimers();
        void updateTimers(uint32_t update_tick);
        inline uint32_t getUpdateTime() const
        {
            return _update_time;
        }
        void initializeUpdateTimer();
        void examineSystemTimers();
        void removeAutoTimer(SystemTimer *timer);
        void addAutoTimer(SystemTimer *timer);
        void setPlayTime(const uint8_t h, const uint8_t m, const uint8_t s)
        {
            _hours_played = h;
            _minutes_played = m;
            _seconds_played = s;
            _milliseconds_played = 0;
        }
        uint8_t getPlayHours() const
        {
            return _hours_played;
        }

        uint8_t getPlayMinutes() const
        {
            return _minutes_played;
        }

        uint8_t getPlaySeconds() const
        {
            return _seconds_played;
        }
        bool isRunning();
        void exitGame()
        {
            _running = false;
        };

    private:
        SystemEngine();
        uint32_t _last_update;
        uint32_t _update_time;
        uint8_t _hours_played;
        uint8_t _minutes_played;
        uint8_t _seconds_played;
        uint16_t _milliseconds_played;
        std::set<SystemTimer *> _auto_system_timers;
        bool _running;
    };
}