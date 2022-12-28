#pragma once

#include "../Core/Game.hpp"
#include "../Core/SceneManager.hpp"
#include "../Core/Timer.hpp"
#include "../Gui/Button.hpp"
#include "../Scenarios/HomeScenery.hpp"
#include <SDL2/SDL_ttf.h>

class MainMenu : public Scene
{
public:
  MainMenu();
  ~MainMenu();

  void onCreate() override;
  void onDestroy() override;

  void processInput(SDL_Event *event);
  void onActivate() override;

  void update() override;
  void draw() override;

private:
  int selected;
  unsigned int homeSceneryId;
  Button btnStartGame;
  Button btnLoadGame;
  Button btnOptions;
  Button btnExit;
};