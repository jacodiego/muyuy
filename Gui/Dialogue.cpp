#include "Dialogue.hpp"

Dialogue::Dialogue()
    : dialogueTexture(Texture{})
{
    font = TTF_OpenFont("Resources/fonts/UbuntuMono-Regular.ttf", 15);
    if (font == NULL)
    {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
    }
    dialogueTexture.loadFromFile("Resources/images/dialog_box.png");
}

Dialogue::~Dialogue()
{
    dialogueTexture.free();
    TTF_CloseFont(font);
    font = NULL;
}

void Dialogue::draw(std::string text)
{
    SDL_Color color = {0, 0, 0};
    SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, 358);
    if (surface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }

    SDL_Texture *fontTexture = SDL_CreateTextureFromSurface(Graphics::getRenderer(), surface);

    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(fontTexture, NULL, NULL, &texW, &texH);

    SDL_Rect Message_rect = {225, 520, texW, texH};

    SDL_FreeSurface(surface);
    dialogueTexture.render(200, 500);
    SDL_RenderCopy(Graphics::getRenderer(), fontTexture, NULL, &Message_rect);

    SDL_DestroyTexture(fontTexture);
};