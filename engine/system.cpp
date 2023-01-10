#include "system.hpp"

namespace muyuy
{

    namespace system
    {

        SystemEngine *systemManager = nullptr;

        //***********************************************************
        //********* SystemTimer
        //***********************************************************

        SystemTimer::SystemTimer() : _state(TimerState::Created),
                                     _auto_update(false),
                                     _duration(0),
                                     _number_loops(0),
                                     _screen_owner(nullptr),
                                     _time_expired(0),
                                     _times_completed(0)
        {
        }

        SystemTimer::SystemTimer(uint32_t duration, int32_t loops) : _state(TimerState::Initialized),
                                                                     _auto_update(false),
                                                                     _duration(duration),
                                                                     _number_loops(loops),
                                                                     _screen_owner(nullptr),
                                                                     _time_expired(0),
                                                                     _times_completed(0)
        {
        }

        SystemTimer::~SystemTimer()
        {
            if (_auto_update)
            {
                systemManager->removeAutoTimer(this);
            }
        }

        void SystemTimer::initialize(uint32_t duration, int32_t number_loops)
        {
            _state = TimerState::Initialized;
            _duration = duration;
            _number_loops = number_loops;
            _time_expired = 0;
            _times_completed = 0;
        }

        void SystemTimer::enableAutoUpdate(screen::GameScreen *owner)
        {
            if (_auto_update)
            {
                // IF_PRINT_WARNING(SYSTEM_DEBUG) << "timer already had auto update enabled" << std::endl;
                return;
            }

            _auto_update = true;
            _screen_owner = owner;
            systemManager->addAutoTimer(this);
        }

        void SystemTimer::enableManualUpdate()
        {
            if (_auto_update == false)
            {
                // IF_PRINT_WARNING(SYSTEM_DEBUG) << "timer was already in manual update mode" << std::endl;
                return;
            }

            systemManager->removeAutoTimer(this);
            _auto_update = false;
            _screen_owner = nullptr;
        }

        void SystemTimer::update()
        {
            if (!isRunning())
            {
                return;
            }

            _updateTimer(systemManager->getUpdateTime());
        }

        void SystemTimer::update(uint32_t time)
        {
            if (_auto_update || !isRunning())
            {
                return;
            }

            _updateTimer(time);
        }

        void SystemTimer::_updateTimer(uint32_t time)
        {
            _time_expired += time;

            if (_time_expired >= _duration)
            {
                _times_completed++;

                if (_number_loops < 0)
                {
                    _time_expired -= _duration;
                }
                else if (_times_completed >= static_cast<uint32_t>(_number_loops))
                {
                    _time_expired = 0;
                    _state = TimerState::Finished;
                }
                else
                {
                    _time_expired -= _duration;
                }
            }
        }

        void SystemTimer::_autoUpdate()
        {
            if (_auto_update == false || !isRunning())
            {
                return;
            }

            _updateTimer(systemManager->getUpdateTime());
        }

        //***********************************************************
        //********* SystemEngine
        //***********************************************************

        SystemEngine::SystemEngine() : _last_update(0), _update_time(1)
        {
        }

        bool SystemEngine::singletonInitialize()
        {
            return true;
        }

        void SystemEngine::initializeTimers()
        {
            _last_update = SDL_GetTicks();
            _update_time = 1;
        }

        void SystemEngine::addAutoTimer(SystemTimer *timer)
        {
            if (timer == nullptr || !timer->isAutoUpdate())
            {
                return;
            }

            _auto_system_timers.insert(timer);
        }

        void SystemEngine::removeAutoTimer(SystemTimer *timer)
        {
            if (timer == nullptr || timer->isAutoUpdate() == false)
            {
                return;
            }

            _auto_system_timers.erase(timer);
        }
    }

}