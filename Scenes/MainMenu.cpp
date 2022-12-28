#include "MainMenu.hpp"

#include <iostream>

MainMenu::MainMenu()
    : selected(1), btnStartGame(Button{"Comenzar", 305, 260}), btnLoadGame(Button{"Cargar", 305, 320}),
      btnOptions(Button{"Opciones", 305, 380}), btnExit(Button{"Salir", 305, 440})
{
}
MainMenu::~MainMenu()
{
}

void MainMenu::onCreate()
{
}

void MainMenu::onActivate()
{
    btnStartGame.selected();
    std::shared_ptr<HomeScenery> homeScenery = std::make_shared<HomeScenery>();
    homeSceneryId = SceneManager::add(homeScenery);
}

void MainMenu::processInput(SDL_Event *event)
{
    switch (event->key.keysym.scancode)
    {

    case SDL_SCANCODE_W:
    case SDL_SCANCODE_UP:
        if (selected > 1)
        {
            selected--;
            btnStartGame.unselected();
            btnLoadGame.unselected();
            btnOptions.unselected();
            btnExit.unselected();
            switch (selected)
            {
            case 1:
                btnStartGame.selected();
                break;
            case 2:
                btnLoadGame.selected();
                break;
            case 3:
                btnOptions.selected();
                break;
            default:
                break;
            }
        }

        break;
    case SDL_SCANCODE_S:
    case SDL_SCANCODE_DOWN:
        if (selected < 4)
        {
            selected++;
            btnStartGame.unselected();
            btnLoadGame.unselected();
            btnOptions.unselected();
            btnExit.unselected();
            switch (selected)
            {
            case 2:
                btnLoadGame.selected();
                break;
            case 3:
                btnOptions.selected();
                break;
            case 4:
                btnExit.selected();
                break;
            default:
                break;
            }
        }
        break;
    case SDL_SCANCODE_RETURN:
        switch (selected)
        {
        case 1:
            btnStartGame.pressed();
            SceneManager::switchTo(homeSceneryId);
            break;
        case 2:
            btnLoadGame.pressed();
            break;
        case 3:
            btnOptions.pressed();
            break;
        case 4:
            btnExit.pressed();
            Game::close();
            break;
        default:
            break;
        }

        break;
    default:
        break;
    }
}

void MainMenu::onDestroy()
{
    btnStartGame.destroy();
    btnLoadGame.destroy();
    btnOptions.destroy();
    btnExit.destroy();
}

void MainMenu::update()
{
}

void MainMenu::draw()
{
    btnStartGame.draw();
    btnLoadGame.draw();
    btnOptions.draw();
    btnExit.draw();
}