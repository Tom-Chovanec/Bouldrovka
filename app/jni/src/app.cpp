#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "include/app.hpp"
#include "include/colors.hpp"
#include "include/icon.hpp"
#include "include/image.hpp"
#include "include/renderer.hpp"
#include "include/simple_button.hpp"
#include "include/text.hpp"

App::App() : m_Renderer(m_Context, m_ResourceManager) {
}

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
        gTheme = LIGHT;
        break;
    case SDL_SYSTEM_THEME_LIGHT:
        gTheme = LIGHT;
        break;
    case SDL_SYSTEM_THEME_DARK:
        gTheme = DARK;
        break;
    }

    m_ResourceManager.loadTexture(m_Context, "icons.png", "icons");
    m_ResourceManager.loadTexture(m_Context, "background_light.png", "background_light");
    m_ResourceManager.loadTexture(m_Context, "background_dark.png", "background_dark");

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
    sprite = {0, 674, 350, 350};

    m_Scenes[INTRO].addObject<Icon>(dst, "icons", sprite, PRIMARY);

    dst = {0.5, 0.75, 0.6, 0.09};

    m_Scenes[INTRO].addObject<SimpleButton>(dst, 0.1, "regular4", "Ideme na to!", PRIMARY, TEXT_LIGHT_LIGHT, [this]{m_CurrentScene = BOULDER_SELECT;});
    Float2 pos = {
        0.5,
        0.55
    };
    m_Scenes[INTRO].addObject<Text>(pos, "regular5", "Vitaj v", TEXT_LIGHT_LIGHT);

    pos.y = 0.6;
    m_Scenes[INTRO].addObject<Text>(pos, "regular6", "Bouldrovke", TEXT_LIGHT_LIGHT);
    // -------- INTRO --------

    // -------- BOULDER SELECT -------
    m_Scenes[BOULDER_SELECT].setBackgroundColor(WHITE);

    dst = {0.88, 0.06, 0.06, 0.06};
    sprite = {414, 5, 128, 128};
    m_Scenes[BOULDER_SELECT].addObject<Icon>(dst, "icons", sprite, BLACK);

    pos = { 0.12, 0.1};
    m_Scenes[BOULDER_SELECT].addObject<Text>(pos, "regular4", "Bouldrové steny", TEXT_DARK_LIGHT, Renderer::TL);
    // -------- BOULDER SELECT -------
}

bool App::isRunning() {
return m_running;
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
            gTheme = LIGHT;
            break;
        case SDL_SYSTEM_THEME_DARK:
            gTheme = DARK;
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
