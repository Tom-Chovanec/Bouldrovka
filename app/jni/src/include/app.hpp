#pragma once

#include <SDL3/SDL.h>
#include <string_view>
#include <unordered_map>

#include "boulder.hpp"
#include "common.hpp"
#include "resource_manager.hpp"
#include "renderer.hpp"
#include "scene.hpp"

class App {
private:
    Context m_Context;
    ResourceManager m_ResourceManager;
    Renderer m_Renderer;

    bool m_running = true;
    SDL_Event m_event;

    enum SCENE {
        NONE = 0,
        INTRO,
        BOULDER_SELECT,
        BOULDER
    } m_CurrentScene = INTRO;

    std::unordered_map<SCENE, Scene> m_Scenes;
    std::unordered_map<std::string_view, Boulder> m_Boulders;

    void changeScene(SCENE scene);

    void setupScenes();

public:
    App();
    ~App();

    bool init();
    bool isRunning();

    void update();

};
