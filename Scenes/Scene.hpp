#pragma once

#include <SDL2/SDL.h>

class Scene
{
public:
  // Called when scene initially created. Called once.
  virtual void onCreate() = 0;

  // Called when scene destroyed. Called at most once (if a scene
  // is not removed from the game, this will never be called).
  virtual void onDestroy() = 0;

  // Called whenever a scene is transitioned into. Can be
  // called many times in a typical game cycle.
  virtual void onActivate(){};

  // Called whenever a transition out of a scene occurs.
  // Can be called many times in a typical game cycle.
  virtual void onDeactivate(){};

  // The below functions can be overridden as necessary in our scenes.
  virtual void processInput(SDL_Event *event){};
  virtual void update(){};
  virtual void lateUpdate(){};
  virtual void draw(){};
};