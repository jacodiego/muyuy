#pragma once

#include <vector>
#include <string>

namespace DialogueManager
{
	void init();
	std::vector<std::string> getDialogue(std::string, std::string);
	std::vector<std::string> getInitDialogue(std::string);
};