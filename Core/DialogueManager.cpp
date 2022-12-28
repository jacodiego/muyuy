#include "DialogueManager.hpp"

#include "rapidjson/document.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <array>

using namespace rapidjson;
using namespace std;

namespace DialogueManager
{
    namespace
    {
        Document jsonDocument;
    }

    void init()
    {
        ifstream dialogues("Resources/data/dialogues.json");

        if (dialogues.fail())
        {
            printf("Unable to load dialogues file!\n");
        }
        else
        {
            ostringstream sstream;
            sstream << dialogues.rdbuf();
            const string str(sstream.str());
            const char *data = str.c_str();
            jsonDocument.Parse(data);
        }
    }

    vector<string> getDialogue(string mapName, string characterName)
    {

        vector<string> dialogue;

        for (auto &maps : jsonDocument.GetArray())
            if (maps["map"].GetString() == mapName)
                for (auto &dialogues : maps["dialogues"].GetArray())
                    if (dialogues.HasMember("character") && dialogues["character"].GetString() == characterName)
                    {
                        for (auto &text : dialogues["texts"].GetArray())
                            dialogue.push_back(text.GetString());
                        goto ready;
                    }

    ready:
        return dialogue;
    }

    vector<string> getInitDialogue(string mapName)
    {

        vector<string> dialogue;

        for (auto &maps : jsonDocument.GetArray())
            if (maps["map"].GetString() == mapName)
                for (auto &dialogues : maps["dialogues"].GetArray())
                    if (dialogues.HasMember("onInit") && dialogues["onInit"].GetBool())
                    {
                        for (auto &text : dialogues["texts"].GetArray())
                            dialogue.push_back(text.GetString());
                        goto ready;
                    }
    ready:
        return dialogue;
    }

}