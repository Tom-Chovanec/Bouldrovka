#include "include/icon.hpp"
#include "include/renderer.hpp"
#include <SDL3/SDL_render.h>

Icon::Icon(const SDL_FRect& rect, const char* textureName, const SDL_FRect& spriteRect, const SDL_Color& color) : m_Rect(rect), m_TextureName(textureName), m_SpriteRect(spriteRect), m_Color(color) {
}

void Icon::render(const Context& context, Renderer& renderer) {
    renderer.renderSprite(m_TextureName, &m_SpriteRect, &m_RenderRect, m_Color);
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