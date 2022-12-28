#include "SceneManager.hpp"
#include <iostream>

namespace SceneManager
{
    namespace
    {
        // Stores all of the scenes associated with this state machine.
        std::unordered_map<unsigned int, std::shared_ptr<Scene>> scenes;

        // Stores a reference to the current scene. Used when drawing/updating.
        std::shared_ptr<Scene> curScene;

        // Stores our current scene id. This is incremented whenever a scene is added.
        unsigned int insertedSceneID;
    }

    void init()
    {
        curScene = 0;
        std::shared_ptr<SplashScreen> splashScreen = std::make_shared<SplashScreen>();
        unsigned int splashScreenID = SceneManager::add(splashScreen);
        std::shared_ptr<MainMenu> mainMenu = std::make_shared<MainMenu>();
        unsigned int mainMenuId = SceneManager::add(mainMenu);
        splashScreen->setSwitchToScene(mainMenuId);
        SceneManager::switchTo(splashScreenID);
    }

    void processInput(SDL_Event *event)
    {
        if (curScene)
        {
            curScene->processInput(event);
        }
    }

    void update()
    {
        if (curScene)
        {
            curScene->update();
        }
    }

    void lateUpdate()
    {
        if (curScene)
        {
            curScene->lateUpdate();
        }
    }

    void draw()
    {
        if (curScene)
        {
            curScene->draw();
        }
    }

    unsigned int add(std::shared_ptr<Scene> scene)
    {
        auto inserted = scenes.insert(std::make_pair(insertedSceneID, scene));

        insertedSceneID++;

        inserted.first->second->onCreate();

        return insertedSceneID - 1;
    }

    void remove(unsigned int id)
    {
        auto it = scenes.find(id);
        if (it != scenes.end())
        {
            if (curScene == it->second)
            {
                // If the scene we are removing is the current scene,
                // we also want to set that to a null pointer so the scene
                // is no longer updated.
                curScene = nullptr;
            }

            // We make sure to call the OnDestroy method
            // of the scene we are removing.
            it->second->onDestroy();

            scenes.erase(it);
        }
    }

    void switchTo(unsigned int id)
    {
        auto it = scenes.find(id);
        if (it != scenes.end())
        {
            if (curScene)
            {
                // If we have a current scene, we call its OnDeactivate method.
                curScene->onDeactivate();
            }

            // Setting the current scene ensures that it is updated and drawn.
            curScene = it->second;

            curScene->onActivate();
        }
    }

    void destroy()
    {
        for (auto it : scenes)
        {
            if (curScene == it.second)
            {
                curScene = nullptr;
            }
            it.second->onDestroy();
        }
        scenes.clear();
    }
}