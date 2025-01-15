#pragma once

#include <SDL3/SDL.h>
#include <functional>
#include <string_view>

#include "colors.hpp"
#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class SimpleButton : public  Object {
private:
    SDL_FRect m_Rect;
    SDL_FRect m_RenderRect;
    Colors::COLOR m_Color;
    Colors::COLOR m_TextColor;
    float m_Radius;
    std::string_view m_FontName;
    std::string_view m_Text;
    std::function<void(void)> m_Callback;

public:
    SimpleButton(
        const SDL_FRect& rect,
        float radius,
        std::string_view fontName,
        std::string_view text,
        Colors::COLOR color,
        Colors::COLOR textColor,
        const std::function<void(void)>& m_Callback = NULL
    );

    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;

    void handleClick(const Context& context) override;
};
