#pragma once

#include <SDL3/SDL.h>
#include <chrono>

struct Float2 {
    float x, y;
};

struct Int2 {
    int x, y;
};

struct Context {
    Int2 windowSize = {1920, 1680};
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    Float2 mousePos = {0, 0};
    std::chrono::duration<float, std::milli> prevTime;
    std::chrono::duration<float, std::milli> deltaTime;
};
