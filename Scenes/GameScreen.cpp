#include "GameScreen.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <iostream>
#include <iomanip>

GameScreen::GameScreen(std::string mapName) : map(new Map(mapName)), dialogueUi(Dialogue{}), secondsDialogue(0.f), secondsAnimation(0.f), animationFrame(0),
                                              countWords(0), dialogue(NULL), countInteractions(0), writingDialogue(""),
                                              state(GameState::LOADING), camera(SDL_Rect{0, 0, 800, 640})
{
}

void GameScreen::onCreate()
{
    const entt::entity hero = Factories::makeHero(reg, Graphics::getSprite("hero"), Coordinate{50, 50});
    const entt::entity woman = Factories::makeCharacter(reg, Graphics::getSprite("woman"), Coordinate{250, 150}, "Leila");
    const entt::entity treasure = Factories::makeTreasure(reg, Graphics::getSprite("treasure"), Coordinate{500, 500}, "potion");
    DialogueManager::init();
}

void GameScreen::onActivate()
{
    dialogue = DialogueManager::getInitDialogue(map->getName());
    if (dialogue.size() > 0)
    {
        state = GameState::DIALOGUING;
        nextDialogue();
    }
    else
    {
        state = GameState::PLAYING;
    }
}

void GameScreen::processInput(SDL_Event *event)
{
    switch (event->type)
    {
    case SDL_KEYDOWN:
        switch (event->key.keysym.scancode)
        {
        case SDL_SCANCODE_SPACE:
            switch (state)
            {
            case GameState::DIALOGUING:
                if (countInteractions < dialogue.size() - 1)
                {
                    countInteractions++;
                    nextDialogue();
                }
                else
                {
                    Interaction::finishDialog(reg);
                    state = GameState::PLAYING;
                }
                break;
            case GameState::PLAYING:
                Interaction::open(reg);
                std::string talkTo = Interaction::talk(reg);
                if (talkTo != "")
                {
                    dialogue = DialogueManager::getDialogue(map->getName(), talkTo);
                    if (dialogue.size() > 0)
                    {
                        countInteractions = 0;
                        countWords = 0;
                        writingDialogue = "";
                        secondsDialogue = 0.f;
                        state = GameState::DIALOGUING;
                        nextDialogue();
                    }
                }
                break;
            }
            break;
        default:
            Keyboard::pressed(reg, event->key.keysym.scancode);
            break;
        }
        break;
    case SDL_KEYUP:
        Keyboard::released(reg, event->key.keysym.scancode);
        break;
    default:
        break;
    }
}

void GameScreen::onDestroy()
{
    delete map;
}

void GameScreen::nextDialogue()
{
    countWords = 0;
    words.clear();
    writingDialogue = "";
    std::istringstream iss(dialogue.at(countInteractions));

    for (std::string token; std::getline(iss, token, ' ');)
    {
        words.push_back(std::move(token));
    }
}

void GameScreen::update()
{
    Movement::hero(reg, camera, map);
    Movement::walkers(reg, map);
    // Intelligence::walkers(reg, map);
    if (state == GameState::DIALOGUING && dialogue.at(countInteractions).size() != writingDialogue.size())
    {
        secondsDialogue += Timer::deltaTime();
        if (writingDialogue.size() == 0 || secondsDialogue >= .05f)
        {
            writingDialogue.append(words[countWords++]);
            if (dialogue.at(countInteractions).size() != writingDialogue.size())
                writingDialogue.append(" ");
            secondsDialogue = 0.f;
        }
    }

    secondsAnimation += Timer::deltaTime();
    if (secondsAnimation >= .14f)
    {
        secondsAnimation = 0.f;
        animationFrame++;

        if (animationFrame > 2)
            animationFrame = 0;
    }
}

void GameScreen::draw()
{
    map->render(camera);
    Render::movable(reg, animationFrame, camera);
    Render::treasures(reg, camera);
    if (state == GameState::DIALOGUING)
        dialogueUi.draw(writingDialogue);
}