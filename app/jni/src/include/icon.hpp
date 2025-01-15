#pragma once

#include <SDL3/SDL.h>
#include <string_view>

#include "colors.hpp"
#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class Icon : public  Object {
private:
    SDL_FRect m_Rect;
    std::string_view m_TextureName;
    SDL_FRect m_SpriteRect;
    SDL_FRect m_RenderRect;
    Colors::COLOR m_Color;

public:
    Icon(const SDL_FRect& rect, std::string_view textureName, const SDL_FRect& spriteRect, Colors::COLOR color);

    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
};
