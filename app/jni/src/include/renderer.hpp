#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <array>
#include <string_view>

#include "resource_manager.hpp"
#include "common.hpp"

class Renderer {
private:
    const Context& m_Context;
    ResourceManager& m_ResourceManager;

    std::map<std::pair<std::string_view, std::string_view>, SDL_Texture*> m_TextTextures;
    std::map<std::pair<std::string_view, std::string_view>, Int2> m_TextSizes;

public:
    Renderer(const Context& context, ResourceManager& resourceManager);

    void renderSprite(
        std::string_view textureName,
        const SDL_FRect* spriteRect,
        const SDL_FRect* dstRect,
        const SDL_Color& color
    ) const;

    void renderImage(std::string_view imageName, const SDL_FRect* dstRect) const;

    void renderRect(const SDL_FRect* rect, const SDL_Color& color);

    void renderRoundedRect(const SDL_FRect* rect, float radius, const SDL_Color& color, std::array<bool, 4> corners = {true, true, true, true});
    void renderRoundedImage(std::string_view imageName, const SDL_FRect* rect, float radius, const SDL_Color& backgroundColor);

    void clearScreen(const SDL_Color& color);

    enum TEXT_ALIGNMENT {
        TL,
        MID
    };

    void renderText(
        std::string_view fontName,
        std::string_view text,
        Float2 pos,
        TEXT_ALIGNMENT textPosition,
        const SDL_Color& color,
        int wrapWidth = 0
    );
};
