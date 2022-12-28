#include "Graphics.hpp"

namespace Graphics
{
    namespace
    {
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SpriteSheet *heroSprite = nullptr;
        SpriteSheet *womanSprite = nullptr;
        SpriteSheet *treasureSprite = nullptr;
        Size screenSize = {800, 640};
        std::map<std::string, SpriteSheet *> sprites;
    }

    void init()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            printf("SDL_Init could not initialize! SDL_Init Error: %s\n", SDL_GetError());
        }
        if (TTF_Init() == -1)
        {
            printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        }
        window = SDL_CreateWindow("Muyui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenSize.w, screenSize.h, false);
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        heroSprite = new SpriteSheet("hero", Size{24, 32}, 12, 3);
        sprites.insert(std::make_pair("hero", heroSprite));
        womanSprite = new SpriteSheet("woman", Size{24, 32}, 12, 3);
        sprites.insert(std::make_pair("woman", womanSprite));
        treasureSprite = new SpriteSheet("treasure", Size{16, 16}, 2, 2);
        sprites.insert(std::make_pair("treasure", treasureSprite));
    }

    void destroy()
    {
        delete heroSprite;
        delete treasureSprite;
        if (renderer != NULL)
            SDL_DestroyRenderer(renderer);
        if (window != NULL)
            SDL_DestroyWindow(window);
    }

    SDL_Renderer *getRenderer()
    {
        return renderer;
    }

    void renderClear()
    {
        SDL_RenderClear(renderer);
    }

    void renderPresent()
    {
        SDL_RenderPresent(renderer);
    }

    Size getScreenSize()
    {
        return screenSize;
    }
    SpriteSheet *getSprite(std::string key)
    {
        return sprites.at(key);
    }
}