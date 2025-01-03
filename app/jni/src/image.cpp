#include "include/image.hpp"
#include "include/colors.hpp"
#include "include/renderer.hpp"
#include <SDL3/SDL_render.h>

Image::Image(
    const SDL_FRect& rect,
    const SDL_FRect& spriteRect,
    const char* textureNameLight,
    const char* textureNameDark
) : 
    m_Rect(rect),
    m_TextureNameLight(textureNameLight),
    m_TextureNameDark(textureNameDark),
    m_SpriteRect(spriteRect)  
{
    if (m_TextureNameDark == "") m_TextureNameDark = m_TextureNameLight;
}

void Image::render(const Context& context, Renderer& renderer) {
    if (gTheme == LIGHT) renderer.renderSprite(m_TextureNameLight, &m_SpriteRect, &m_RenderRect, SDL_Color{255, 255, 255, 255});
    else renderer.renderSprite(m_TextureNameDark, &m_SpriteRect, &m_RenderRect, SDL_Color{255, 255, 255, 255});
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
