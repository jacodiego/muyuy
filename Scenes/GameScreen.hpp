#pragma once

#include "Scene.hpp"
#include "../Core/Timer.hpp"
#include "../Core/DialogueManager.hpp"
#include "../Core/Factories.hpp"
#include "../Gui/Dialogue.hpp"
#include "../Gui/Map.hpp"
#include "../Gui/SpriteSheet.hpp"
#include "../Systems/Movement.hpp"
#include "../Systems/Keyboard.hpp"
#include "../Systems/Interaction.hpp"
#include "../Systems/Intelligence.hpp"
#include "../Systems/Render.hpp"
#include <entt/entity/registry.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <iostream>
#include <iomanip>

enum class GameState
{
  LOADING,
  PLAYING,
  DIALOGUING,
  FIGHTING,
  ACTING
};

class GameScreen : public Scene
{
public:
  GameScreen(std::string);

  void onCreate() override;
  void onDestroy() override;
  void onActivate() override;
  void processInput(SDL_Event *event);
  void update() override;
  void draw() override;
  void nextDialogue();

private:
  GameState state;
  Dialogue dialogueUi;
  std::vector<std::string> dialogue;
  std::string textDialogue;
  std::vector<std::string> words;
  std::string writingDialogue;
  int countWords;
  int countInteractions;
  float secondsDialogue;
  float secondsAnimation;
  int animationFrame;
  Map *map;
  SDL_Rect camera;
  entt::registry reg;
};