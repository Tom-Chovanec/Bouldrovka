#pragma once

#include <SDL3/SDL.h>
#include <chrono>

struct Float2 {
    float x, y;

    Float2 operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }
};

struct Int2 {
    int x, y;

    Int2 operator*(int scalar) const {
        return {x * scalar, y * scalar};
    }
};

struct Context {
    Int2 windowSize = {1920, 1680};
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_FPoint mousePos = {0, 0};
    std::chrono::duration<float, std::milli> prevTime;
    std::chrono::duration<float, std::milli> deltaTime;
};
