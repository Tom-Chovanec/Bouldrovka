#pragma once

#include <SDL3/SDL.h>

#include "resource_manager.hpp"
#include "common.hpp"

class Renderer {
private:
    const Context& m_Context;
    ResourceManager& m_ResourceManager;

public:
    Renderer(const Context& context, ResourceManager& resourceManager);

    void renderSprite(
        const char* textureName,
        const SDL_FRect* spriteRect,
        const SDL_FRect* dstRect,
        const SDL_Color& color
    );

    void renderRect( const SDL_FRect* rect, const SDL_Color& color);
};
