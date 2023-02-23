#pragma once

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <map>
#include <string>
#include <vector>
#include "utils/singleton.hpp"

namespace muyuy::script
{
    class ScriptEngine;
    extern ScriptEngine *scriptManager;

    class ScriptEngine : public utils::Singleton<ScriptEngine>
    {
        friend class utils::Singleton<ScriptEngine>;

    public:
        bool singletonInitialize();
        ~ScriptEngine();
        sol::state &getGlobalState();
        static std::string getTableSpace(const std::string &filename)
        {
            if (filename.empty())
                return std::string();

            int32_t period = filename.find(".");
            int32_t last_slash = filename.find_last_of("/");
            std::string tablespace = filename.substr(last_slash + 1, period - (last_slash + 1));
            return tablespace;
        }

    private:
        ScriptEngine();
        sol::state _global_state;
    };

    class ScriptReader
    {
    public:
        ScriptReader(const std::string &);
        bool openTable(const std::string &);
        int readInt(const std::string &);
        bool run(const std::string &);
        void closeTable();

    private:
        sol::state &lua;
        std::vector<std::string> _tables_open;
    };
}