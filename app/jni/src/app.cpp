#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "include/app.hpp"
#include "include/boulder_selection.hpp"
#include "include/colors.hpp"
#include "include/icon.hpp"
#include "include/image.hpp"
#include "include/renderer.hpp"
#include "include/simple_button.hpp"
#include "include/text.hpp"
#include "include/hold_selection.hpp"


App::App() : m_Renderer(m_Context, m_ResourceManager) {}

App::~App() {
    SDL_DestroyRenderer(m_Context.renderer);
    SDL_DestroyWindow(m_Context.window);
    TTF_Quit();
    SDL_Quit();
}

bool App::init() {
    if (!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init failed (%s)", SDL_GetError());
        return false;
    }
    if (!TTF_Init()) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "TTF_Init failed (%s)", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(
        "Bouldrovka",
        m_Context.windowSize.x,
        m_Context.windowSize.y,
        SDL_WINDOW_EXTERNAL | SDL_WINDOW_FULLSCREEN,
        &m_Context.window,
        &m_Context.renderer
    )) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to CreateWindowAndRenderer, error: %s", SDL_GetError());
        return false;
    }

    switch (SDL_GetSystemTheme()) {
    case SDL_SYSTEM_THEME_UNKNOWN:
        Colors::gTheme = Colors::LIGHT;
        break;
    case SDL_SYSTEM_THEME_LIGHT:
        Colors::gTheme = Colors::LIGHT;
        break;
    case SDL_SYSTEM_THEME_DARK:
        Colors::gTheme = Colors::DARK;
        break;
    }

    m_ResourceManager.loadTexture(m_Context, "atlas1.png", "atlas1");
    m_ResourceManager.loadTexture(m_Context, "atlas2.png", "atlas2");
    m_ResourceManager.loadTexture(m_Context, "background_light.png", "background_light");
    m_ResourceManager.loadTexture(m_Context, "background_dark.png", "background_dark");

    m_ResourceManager.loadFont(m_Context, "fonts/JosefinSans-Regular.ttf", "regular2", 0.02);
    m_ResourceManager.loadFont(m_Context, "fonts/JosefinSans-Regular.ttf", "regular4", 0.04);
    m_ResourceManager.loadFont(m_Context, "fonts/JosefinSans-Regular.ttf", "regular5", 0.05);
    m_ResourceManager.loadFont(m_Context, "fonts/JosefinSans-Regular.ttf", "regular6", 0.06);

    setupScenes();

    return true;
}


void App::setupScenes() {
    // -------- INTRO --------
    SDL_FRect dst = {0, 0, 1, 1};
    SDL_FRect sprite = {0, 0, 1230, 2700};
    m_Scenes[INTRO].addObject<Image>(dst, sprite, "background_light", "background_dark");

    dst = {0.5, 0.2, 0.2, 0.2};
    sprite = {537, 5, 482, 482};

    m_Scenes[INTRO].addObject<Icon>(dst, "atlas2", sprite, Colors::COLOR::PRIMARY);

    dst = {0.5, 0.75, 0.6, 0.09};

    m_Scenes[INTRO].addObject<SimpleButton>(dst, 0.1, "regular4", "Ideme na to!", Colors::COLOR::PRIMARY, Colors::COLOR::TEXT_LIGHT_LIGHT, [this]{changeScene(BOULDER_SELECT);});
    Float2 pos = {
        0.5,
        0.55
    };
    m_Scenes[INTRO].addObject<Text>(pos, "regular5", "Vitaj v", Colors::COLOR::TEXT_LIGHT_LIGHT);

    pos.y = 0.6;
    m_Scenes[INTRO].addObject<Text>(pos, "regular6", "Bouldrovke", Colors::COLOR::TEXT_LIGHT_LIGHT);
    // -------- INTRO --------

    // -------- BOULDER SELECT -------
    m_Scenes[BOULDER_SELECT].setBackgroundColor(Colors::COLOR::WHITE);

    dst = {0.88, 0.06, 0.06, 0.06};
    sprite = {414, 5, 128, 128};
    m_Scenes[BOULDER_SELECT].addObject<Icon>(dst, "atlas1", sprite, Colors::COLOR::BLACK);

    pos = { 0.06, 0.1};
    m_Scenes[BOULDER_SELECT].addObject<Text>(pos, "regular4", "Bouldrové steny", Colors::COLOR::TEXT_DARK_LIGHT, Renderer::TL);

    dst = {0.06, 0.175, 0.88, 0.25};
    m_Scenes[BOULDER_SELECT].addObject<BoulderSelection>("background_light", dst, [this]{changeScene(BOULDER);});

    dst.y += 0.3;
    m_Scenes[BOULDER_SELECT].addObject<BoulderSelection>("", dst);

    // -------- BOULDER SELECT -------

    // -------- BOULDER --------
    dst = {0.88, 0.06, 0.06, 0.06};
    sprite = {414, 5, 128, 128};
    m_Scenes[BOULDER].addObject<Icon>(dst, "atlas1", sprite, Colors::COLOR::BLACK);

    dst = {0.09, 0.06, 0.06, 0.06};
    sprite = {552, 143, 128, 128};
    m_Scenes[BOULDER].addObject<Icon>(dst, "atlas1", sprite, Colors::COLOR::BLACK, [this]{changeScene(BOULDER_SELECT);});

    pos = {0.06, 0.1};
    m_Scenes[BOULDER].addObject<Text>(pos, "regular4", "Drevený boulder", Colors::COLOR::TEXT_DARK_LIGHT, Renderer::TL);

    dst = {0, 0.2, 1, 0.8};
    sprite = {0, 0, 1230, 2700};
    m_Scenes[BOULDER].addObject<Image>(dst, sprite, "background_light", "background_dark");


    dst = {0.85, 0.25, 0.15, 0.35};
    m_Scenes[BOULDER].addObject<HoldSelection>(dst);
    // -------- BOULDER --------
}

bool App::isRunning() {
    return m_running;
}

void App::changeScene(SCENE scene) {
    m_CurrentScene = scene;
}

void App::update() {
m_Context.mousePos = {-1, -1};
SDL_PollEvent(&m_event); {
    switch(m_event.type) {
    case (SDL_EVENT_QUIT):
        m_running = false;
        break;
    case (SDL_EVENT_WINDOW_RESIZED):
        SDL_GetWindowSize(m_Context.window, &m_Context.windowSize.x, &m_Context.windowSize.y);
        for (auto& scene : m_Scenes) {
            scene.second.update(m_Context);
        }
        break;
    case (SDL_EVENT_SYSTEM_THEME_CHANGED):
        switch (SDL_GetSystemTheme()) {
        case SDL_SYSTEM_THEME_UNKNOWN:
            break;
        case SDL_SYSTEM_THEME_LIGHT:
            Colors::gTheme = Colors::LIGHT;
            break;
        case SDL_SYSTEM_THEME_DARK:
            Colors::gTheme = Colors::DARK;
            break;
        }
        break;
    case (SDL_EVENT_FINGER_UP):
        m_Context.mousePos.x = m_event.tfinger.x * m_Context.windowSize.x;
        m_Context.mousePos.y = m_event.tfinger.y * m_Context.windowSize.y;
        break;
    } }
    m_Scenes[m_CurrentScene].handleClick(m_Context);
    m_Scenes[m_CurrentScene].render(m_Context, m_Renderer);
    SDL_RenderPresent(m_Context.renderer);
}
