#pragma once

#include <SDL3/SDL.h>

#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class Image : public  Object {
private:
    SDL_FRect m_Rect;
    std::string m_TextureNameDark;
    std::string m_TextureNameLight;
    SDL_FRect m_SpriteRect;
    SDL_FRect m_RenderRect;

public:
    Image(
        const SDL_FRect& rect,
        const SDL_FRect& spriteRect,
        const char* textureNameLight,
        const char* textureNameDark = ""
    );

    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
};
