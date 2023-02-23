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
        _global_state.open_libraries(sol::lib::base, sol::lib::math);
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

    ScriptReader::ScriptReader(const std::string &pathname) : lua(scriptManager->getGlobalState())
    {
        lua.open_libraries(sol::lib::base);
        lua.script_file(pathname);
    }

    bool ScriptReader::openTable(const std::string &tablename)
    {
        _tables_open.push_back(tablename);
        return true;
    }

    void ScriptReader::closeTable()
    {
        _tables_open.pop_back();
    }

    int ScriptReader::readInt(const std::string &value)
    {
        sol::table table;
        for (int i = 0; i < _tables_open.size(); i++)
        {
            if (i == 0)
                table = lua[_tables_open[i]];
            else
                table = table[_tables_open[i]];
        }
        return table[value].get<int>();
    }

    bool ScriptReader::run(const std::string &value)
    {
        sol::function fx = lua[value];
        fx();
        return true;
    }

}