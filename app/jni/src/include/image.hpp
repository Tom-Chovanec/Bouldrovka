#pragma once

#include <SDL3/SDL.h>

#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class Image : public  Object {
private:
    SDL_FRect m_Rect;
    const char* m_TextureNameDark;
    const char* m_TextureNameLight;
    SDL_FRect m_SpriteRect;
    SDL_FRect m_RenderRect;
    SDL_Color m_Color;

public:
    Image(const SDL_FRect& rect, const SDL_FRect& spriteRect, const char* textureNameLight, const char* textureNameDark, const SDL_Color& color = SDL_Color{255, 255, 255, 255});

    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
};
