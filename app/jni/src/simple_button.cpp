#include "include/simple_button.hpp"
#include "include/renderer.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

SimpleButton::SimpleButton(
    const SDL_FRect& rect,
    float radius,
    std::string_view fontName,
    std::string_view text,
    Colors::COLOR color,
    Colors::COLOR textColor,
    const std::function<void(void)>& callback
) : 
    m_Rect(rect),
    m_Radius(radius),
    m_FontName(fontName),
    m_Text(text),
    m_Color(color),
    m_TextColor(textColor),
    m_Callback(callback) {
}

void SimpleButton::render(const Context& context, Renderer& renderer) {

    renderer.renderRoundedRect(&m_RenderRect, m_Radius, getColor(m_Color));
    renderer.renderText(
        m_FontName,
        m_Text,
        {
            m_RenderRect.x + m_RenderRect.w / 2,
            m_RenderRect.y + m_RenderRect.h / 2
        },
        Renderer::MID,
        getColor(m_TextColor)
    );

}

void SimpleButton::update(const Context& context) {
    float w, h;
    w = m_Rect.w * context.windowSize.x;
    h = m_Rect.h * context.windowSize.y;
    m_Radius *= context.windowSize.x;

    m_RenderRect = {
        m_Rect.x * context.windowSize.x - w / 2,
        m_Rect.y * context.windowSize.y - h / 2,
        w,
        h
    };
}

void SimpleButton::handleClick(const Context& context) {
    if (SDL_PointInRectFloat(&context.mousePos, &m_RenderRect)) {
        m_Callback();
    }
}
