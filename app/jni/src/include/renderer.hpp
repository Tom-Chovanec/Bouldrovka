#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "resource_manager.hpp"
#include "common.hpp"

class Renderer {
private:
    const Context& m_Context;
    ResourceManager& m_ResourceManager;

    std::map<std::pair<const std::string&, const char*>, SDL_Texture*> m_TextTextures;
    std::map<std::pair<const std::string&, const char*>, Int2> m_TextSizes;

public:
    Renderer(const Context& context, ResourceManager& resourceManager);

    void renderSprite(
        const char* textureName,
        const SDL_FRect* spriteRect,
        const SDL_FRect* dstRect,
        const SDL_Color& color
    );

    void renderRect(const SDL_FRect* rect, const SDL_Color& color);

    void clearScreen(const SDL_Color& color);

    enum TEXT_ALIGNMENT {
        TL,
        MID
    };

    void renderText(
        const char* fontName,
        const std::string& text,
        Float2 pos,
        TEXT_ALIGNMENT textPosition,
        const SDL_Color& color,
        int wrapWidth = 0
    );
};
