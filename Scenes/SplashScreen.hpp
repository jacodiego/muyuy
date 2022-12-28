#pragma once

#include "../Core/SceneManager.hpp"
#include "../Core/Timer.hpp"
#include "../Gui/Texture.hpp"

class SplashScreen : public Scene
{
public:
  SplashScreen();
  ~SplashScreen();

  void onCreate() override;
  void onDestroy() override;
  void onActivate() override;
  void update() override;
  void draw() override;

  void setSwitchToScene(unsigned int id);

private:
  Texture splashTexture;
  float showForSeconds;
  float currentSeconds;
  Uint8 alpha;
  unsigned int switchToState;
};