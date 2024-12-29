#include "include/simple_button.hpp"
#include "include/renderer.hpp"
#include <SDL3/SDL_render.h>

SimpleButton::SimpleButton(
    const SDL_FRect& rect,
    float radius,
    const char* fontName,
    const std::string& text,
    const SDL_Color& color,
    const SDL_Color& textColor
) : 
    m_Rect(rect),
    m_Radius(radius),
    m_FontName(fontName),
    m_Text(text),
    m_Color(color),
    m_TextColor(textColor) {
}

void SimpleButton::render(const Context& context, Renderer& renderer) {

    //top left
    SDL_FRect cornerPos = {
        m_RenderRect.x,
        m_RenderRect.y,
        m_Radius,
        m_Radius
    };
    SDL_FRect sprite = {763, 763, 256, 256};
    renderer.renderSprite("icons", &sprite, &cornerPos, m_Color);

    //top right
    cornerPos.x = m_RenderRect.x + m_RenderRect.w - m_Radius;
    sprite = {497, 763, 256, 256};
    renderer.renderSprite("icons", &sprite, &cornerPos, m_Color);

    //bottom right
    cornerPos.y = m_RenderRect.y + m_RenderRect.h - m_Radius;
    sprite = {497, 497, 256, 256};
    renderer.renderSprite("icons", &sprite, &cornerPos, m_Color);

    //bottom left
    cornerPos.x = m_RenderRect.x;
    sprite = {763, 497, 256, 256};
    renderer.renderSprite("icons", &sprite, &cornerPos, m_Color);

    SDL_FRect filler = {
        m_RenderRect.x + m_Radius,
        m_RenderRect.y,
        m_RenderRect.w - m_Radius * 2,
        m_RenderRect.h
    };
    renderer.renderRect(&filler, m_Color);

    filler = {
        m_RenderRect.x,
        m_RenderRect.y + m_Radius,
        m_RenderRect.w,
        m_RenderRect.h - m_Radius * 2
    };
    renderer.renderRect(&filler, m_Color);

    renderer.renderText(
        m_FontName,
        m_Text,
        m_RenderRect.x + m_RenderRect.w / 2,
        m_RenderRect.y + m_RenderRect.h / 2,
        Renderer::MID,
        m_TextColor
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
