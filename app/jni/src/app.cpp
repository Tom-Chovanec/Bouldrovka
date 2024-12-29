#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "include/app.hpp"
#include "include/icon.hpp"
#include "include/image.hpp"
#include "include/simple_button.hpp"

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

    m_ResourceManager.loadTexture(m_Context, "icons.png", "icons");
    m_ResourceManager.loadTexture(m_Context, "background.png", "background");

    m_ResourceManager.loadFont(m_Context, "fonts/JosefinSans-Regular.ttf", "basic", 0.04);

    SDL_FRect dst = {0, 0, 1, 1};
    SDL_FRect sprite = {0, 0, 1230, 2700};
    const char* name =  "background";

    m_Scenes[INTRO].addObject<Image>(dst, name, sprite);

    dst = {0.5, 0.2, 0.2, 0.2};
    sprite = {0, 674, 350, 350};
    name =  "icons";
    SDL_Color color = {255, 106, 20, 255};

    m_Scenes[INTRO].addObject<Icon>(dst, name, sprite, color);

    dst = {0.5, 0.75, 0.6, 0.09};

    m_Scenes[INTRO].addObject<SimpleButton>(dst, 0.1, "basic", "Ideme na to!", color);


 

    return true;
}

bool App::isRunning() {
    return m_running;
}

void App::update() {
    SDL_PollEvent(&m_event); {
        switch(m_event.type) {
        case (SDL_EVENT_QUIT):
            m_running = false;
        case (SDL_EVENT_WINDOW_RESIZED):
            SDL_GetWindowSize(m_Context.window, &m_Context.windowSize.x, &m_Context.windowSize.y);
            m_Scenes[INTRO].update(m_Context);
        }
    }
    SDL_SetRenderDrawColor(m_Context.renderer, 0x18, 0x18, 0x18, 255);
    SDL_RenderClear(m_Context.renderer);
    m_Scenes[INTRO].render(m_Context, m_Renderer);
    SDL_RenderPresent(m_Context.renderer);
}
