#include "include/image.hpp"
#include "include/renderer.hpp"
#include <SDL3/SDL_render.h>

Image::Image(const SDL_FRect& rect, const char* textureName, const SDL_FRect& spriteRect, const SDL_Color& color) : m_Rect(rect), m_TextureName(textureName), m_SpriteRect(spriteRect), m_Color(color) {
}

void Image::render(const Context& context, Renderer& renderer) {
    renderer.renderSprite(m_TextureName, &m_SpriteRect, &m_RenderRect, m_Color);
}

void Image::update(const Context& context) {
    float w, h;

    m_RenderRect = {
        m_Rect.x * context.windowSize.x,
        m_Rect.y * context.windowSize.y,
        m_Rect.w * context.windowSize.x,
        m_Rect.h * context.windowSize.y
    };
}
