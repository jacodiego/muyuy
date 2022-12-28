#include "SplashScreen.hpp"
#include <iostream>
#include <iomanip>

SplashScreen::SplashScreen()
    : switchToState(0), currentSeconds(0.f),
      showForSeconds(4.f), alpha(0), splashTexture(Texture{})
{
}

SplashScreen::~SplashScreen()
{
    splashTexture.free();
}

void SplashScreen::onCreate()
{
    splashTexture.loadFromFile("Resources/images/splash.png");
}

void SplashScreen::onActivate()
{
    currentSeconds = 0.f;
}

void SplashScreen::onDestroy()
{
    splashTexture.free();
}

void SplashScreen::setSwitchToScene(unsigned int id)
{
    switchToState = id;
}

void SplashScreen::update()
{
    currentSeconds += Timer::deltaTime();
    if (currentSeconds <= 1.f)
    {
        alpha = (int)(currentSeconds * 255);
    }
    if (currentSeconds >= (showForSeconds - 1.f))
    {
        alpha = (int)((1 - (currentSeconds - (showForSeconds - 1.f))) * 255);
    }

    if (currentSeconds >= showForSeconds)
    {
        SceneManager::switchTo(switchToState);
    }
}

void SplashScreen::draw()
{
    splashTexture.setAlpha(alpha);
    splashTexture.render(250, 149);
}