#include "script.hpp"

namespace muyuy::script
{

    ScriptEngine *scriptManager = nullptr;

    //-----------------------------------------------------------------
    //  ScriptEngine
    //-----------------------------------------------------------------

    ScriptEngine::ScriptEngine()
    {
        sol::state _global_state;
        _global_state.open_libraries(sol::lib::base);
    };

    ScriptEngine::~ScriptEngine()
    {
    }

    bool ScriptEngine::singletonInitialize()
    {
        return true;
    }

    sol::state &ScriptEngine::getGlobalState()
    {
        return _global_state;
    }
}