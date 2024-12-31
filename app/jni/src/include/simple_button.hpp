#pragma once

#include <SDL3/SDL.h>
#include <functional>

#include "colors.hpp"
#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class SimpleButton : public  Object {
private:
    SDL_FRect m_Rect;
    SDL_FRect m_RenderRect;
    COLORS m_Color;
    COLORS m_TextColor;
    float m_Radius;
    const char* m_FontName;
    std::string m_Text;
    std::function<void(void)> m_Callback;

public:
    SimpleButton(
        const SDL_FRect& rect,
        float radius,
        const char* fontName,
        const std::string& text,
        COLORS color,
        COLORS textColor,
        const std::function<void(void)>& m_Callback = NULL
    );

    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;

    void handleClick(const Context& context) override;
};
