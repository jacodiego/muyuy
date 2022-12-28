#pragma once

#include "../Core/Graphics.hpp"
#include "../Core/Timer.hpp"
#include "Texture.hpp"
#include <SDL2/SDL_ttf.h>
#include <iostream>

enum class ButtonState
{
    NORMAL,
    SELECTED,
    PRESSED,
};

class Button
{
public:
    Button(const char *, int, int);
    void destroy();
    void unselected() { btnState = ButtonState::NORMAL; };
    void selected() { btnState = ButtonState::SELECTED; };
    void pressed() { btnState = ButtonState::PRESSED; };
    void draw();

private:
    const char *text;
    int x;
    int y;
    float btnPressedTime;
    ButtonState btnState;
    Texture btnTexture;
    Texture btnSelectedTexture;
    Texture btnPressedTexture;
    SDL_Texture *fontTexture;
    SDL_Surface *surface;
    SDL_Rect Message_rect;
    TTF_Font *font;
};