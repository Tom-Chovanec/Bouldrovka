#include "include/renderer.hpp"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

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


void Renderer::clearScreen(const SDL_Color& color) {
    SDL_SetRenderDrawColor(m_Context.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_Context.renderer);
}

void Renderer::renderRect( const SDL_FRect* rect, const SDL_Color& color) {
    SDL_SetRenderDrawColor(m_Context.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_Context.renderer, rect);
}

void Renderer::renderText(
    const char* fontName,
    const std::string& text,
    Float2 pos,
    TEXT_ALIGNMENT textPosition,
    const SDL_Color& color,
    int wrapWidth
) {
    if (m_TextTextures.count({text, fontName}) == 0) {
        SDL_Surface* srf = TTF_RenderText_Blended_Wrapped(m_ResourceManager.getFont(fontName), text.c_str(), text.length(), SDL_Color{255, 255, 255, 255}, wrapWidth);
        m_TextTextures[{text, fontName}] = SDL_CreateTextureFromSurface(m_Context.renderer, srf);
        m_TextSizes[{text, fontName}] = {srf->w, srf->h};
        SDL_DestroySurface(srf);
    }

    SDL_FRect dst;

    switch (textPosition) {
        case (TL):
            dst.x = pos.x;
            dst.y = pos.y;
            dst.w = m_TextSizes[{text, fontName}].x;
            dst.h = m_TextSizes[{text, fontName}].y;
            break;

        case (MID):
            float w = m_TextSizes[{text, fontName}].x;
            float h = m_TextSizes[{text, fontName}].y;
            dst.x = pos.x - w / 2;
            dst.y = pos.y - h / 2;
            dst.w = w;
            dst.h = h;
            break;
    }

    SDL_SetTextureColorMod(m_TextTextures[{text, fontName}], color.r, color.g, color.b);
    SDL_RenderTexture(m_Context.renderer, m_TextTextures[{text, fontName}], NULL, &dst);

}
