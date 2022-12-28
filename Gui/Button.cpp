#include "Button.hpp"

Button::Button(const char *text, int x, int y)
    : text(text), x(x), y(y), btnTexture(Texture{}), btnSelectedTexture(Texture{}),
      btnPressedTexture(Texture{}), btnPressedTime(0.f), btnState(ButtonState::NORMAL)
{
    font = TTF_OpenFont("Resources/fonts/UbuntuMono-Regular.ttf", 24);
    if (font == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    btnTexture.loadFromFile("Resources/images/button.png");
    btnSelectedTexture.loadFromFile("Resources/images/button_selected.png");
    btnPressedTexture.loadFromFile("Resources/images/button_pressed.png");
    SDL_Color color = {50, 50, 50};
    surface = TTF_RenderText_Blended(font, text, color);
    if (surface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }

    fontTexture = SDL_CreateTextureFromSurface(Graphics::getRenderer(), surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(fontTexture, NULL, NULL, &texW, &texH);

    Message_rect = {x + (btnTexture.getWidth() - texW) / 2, y + (btnTexture.getHeight() - texH) / 2, texW, texH};

    SDL_FreeSurface(surface);
}

void Button::destroy()
{
    btnTexture.free();
    btnSelectedTexture.free();
    btnPressedTexture.free();
    TTF_CloseFont(font);
    font = NULL;
    SDL_DestroyTexture(fontTexture);
    fontTexture = NULL;
}

void Button::draw()
{
    if (btnState == ButtonState::PRESSED)
    {
        btnPressedTime += Timer::deltaTime();
        if (btnPressedTime > .25f)
        {
            btnState = ButtonState::SELECTED;
            btnPressedTime = 0.f;
        }
    }
    switch (btnState)
    {
    case ButtonState::NORMAL:
        btnTexture.render(x, y);
        break;
    case ButtonState::SELECTED:
        btnSelectedTexture.render(x, y);
        break;
    case ButtonState::PRESSED:
        btnPressedTexture.render(x, y);
        break;
    default:
        break;
    }
    SDL_RenderCopy(Graphics::getRenderer(), fontTexture, NULL, &Message_rect);
};