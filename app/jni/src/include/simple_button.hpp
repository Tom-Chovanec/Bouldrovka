#pragma once

#include <SDL3/SDL.h>
#include <functional>

#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class SimpleButton : public  Object {
private:
    SDL_FRect m_Rect;
    SDL_FRect m_RenderRect;
    SDL_Color m_Color;
    SDL_Color m_TextColor;
    float m_Radius;
    const char* m_FontName;
    std::string m_Text;

public:
    SimpleButton(
        const SDL_FRect& rect,
        float radius,
        const char* fontName,
        const std::string& text,
        const SDL_Color& color = {255, 255, 255, 0},
        const SDL_Color& textColor = {255, 255, 255, 255}
    );

    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
};
