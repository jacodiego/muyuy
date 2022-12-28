#pragma once

#include <memory>
#include <unordered_map>

#include "../Scenes/Scene.hpp"
#include "../Scenes/SplashScreen.hpp"
#include "../Scenes/MainMenu.hpp"

namespace SceneManager
{
	void init();
	void processInput(SDL_Event *event);
	void update();
	void lateUpdate();
	void draw();
	unsigned int add(std::shared_ptr<Scene> scene);
	void switchTo(unsigned int id);
	void remove(unsigned int id);
	void destroy();
};