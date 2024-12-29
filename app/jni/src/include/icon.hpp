#pragma once

#include <SDL3/SDL.h>

#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class Icon : public  Object {
private:
    SDL_FRect m_Rect;
    const char* m_TextureName;
    SDL_FRect m_SpriteRect;
    SDL_FRect m_RenderRect;
    SDL_Color m_Color;

public:
    Icon(const SDL_FRect& rect, const char* textureName, const SDL_FRect& spriteRect, const SDL_Color& color = {255, 255, 255, 0});

    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
};
