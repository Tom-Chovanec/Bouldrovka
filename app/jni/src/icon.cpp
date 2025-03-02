#include "include/icon.hpp"
#include "include/renderer.hpp"
#include <SDL3/SDL_render.h>
#include <string_view>

Icon::Icon(const SDL_FRect& rect, std::string_view textureName, const SDL_FRect& spriteRect, Colors::COLOR color, const std::function<void(void)> callback)
    : m_Rect(rect),
      m_TextureName(textureName),
      m_SpriteRect(spriteRect),
      m_Color(color),
      m_Callback(callback)
{
}

void Icon::render(const Context& context, Renderer& renderer) {
    renderer.renderSprite(m_TextureName, &m_SpriteRect, &m_RenderRect, getColor(m_Color));
}

void Icon::update(const Context& context) {
    float w, h;
    w = m_Rect.w * context.windowSize.x;
    h = m_Rect.h * context.windowSize.x;

    m_RenderRect = {
        m_Rect.x * context.windowSize.x - w / 2,
        m_Rect.y * context.windowSize.y - h / 2,
        w,
        h
    };
}

void Icon::handleClick(const Context& context) {
    if (SDL_PointInRectFloat(&context.mousePos, &m_RenderRect)) {
        m_Callback();
    }
}
