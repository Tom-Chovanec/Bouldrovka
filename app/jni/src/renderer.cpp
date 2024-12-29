#include "include/renderer.hpp"
#include <SDL3/SDL_render.h>

Renderer::Renderer(const Context& context, ResourceManager& resourceManager) : 
    m_Context(context),
    m_ResourceManager(resourceManager) {
}

void Renderer::renderSprite(
    const char* textureName,
    const SDL_FRect* spriteRect,
    const SDL_FRect* dstRect,
    const SDL_Color& color
) {
    SDL_SetTextureColorMod(m_ResourceManager.getTexture(textureName), color.r, color.g, color.b);
    SDL_RenderTexture(m_Context.renderer, m_ResourceManager.getTexture(textureName), spriteRect, dstRect);
}
