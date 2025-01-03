#pragma once

#include <SDL3/SDL.h>

#include "colors.hpp"
#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class Icon : public  Object {
private:
    SDL_FRect m_Rect;
    std::string m_TextureName;
    SDL_FRect m_SpriteRect;
    SDL_FRect m_RenderRect;
    COLORS m_Color;

public:
    Icon(const SDL_FRect& rect, const std::string& textureName, const SDL_FRect& spriteRect, COLORS color);

    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
};
