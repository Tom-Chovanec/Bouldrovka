#include "include/renderer.hpp"

Renderer::Renderer(const Context& context, ResourceManager& resourceManager) : 
    m_Context(context),
    m_ResourceManager(resourceManager) {
}

void Renderer::renderSprite(
    std::string_view textureName,
    const SDL_FRect* spriteRect,
    const SDL_FRect* dstRect,
    const SDL_Color& color
) const {
    SDL_SetTextureColorMod(m_ResourceManager.getTexture(textureName), color.r, color.g, color.b);
    SDL_RenderTexture(m_Context.renderer, m_ResourceManager.getTexture(textureName), spriteRect, dstRect);
}

void Renderer::renderImage(std::string_view imageName, const SDL_FRect* dstRect) const {
    SDL_RenderTexture(m_Context.renderer, m_ResourceManager.getTexture(imageName), NULL, dstRect);
}


void Renderer::clearScreen(const SDL_Color& color) {
    SDL_SetRenderDrawColor(m_Context.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_Context.renderer);
}

void Renderer::renderRect( const SDL_FRect* rect, const SDL_Color& color) {
    SDL_SetRenderDrawColor(m_Context.renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(m_Context.renderer, rect);
}

void Renderer::renderRoundedRect(const SDL_FRect* rect, float radius, const SDL_Color& color, std::array<bool, 4> corners) {
    SDL_FRect cornerPos;
    SDL_FRect sprite;

    // Top-left corner
    if (corners[0]) {
        cornerPos = {rect->x, rect->y, radius, radius};
        sprite = {763, 763, 256, 256};
        renderSprite("atlas1", &sprite, &cornerPos, color);
    }

    // Top-right corner
    if (corners[1]) {
        cornerPos = {rect->x + rect->w - radius, rect->y, radius, radius};
        sprite = {497, 763, 256, 256};
        renderSprite("atlas1", &sprite, &cornerPos, color);
    }

    // Bottom-right corner
    if (corners[2]) {
        cornerPos = {rect->x + rect->w - radius, rect->y + rect->h - radius, radius, radius};
        sprite = {497, 497, 256, 256};
        renderSprite("atlas1", &sprite, &cornerPos, color);
    }

    // Bottom-left corner
    if (corners[3]) {
        cornerPos = {rect->x, rect->y + rect->h - radius, radius, radius};
        sprite = {763, 497, 256, 256};
        renderSprite("atlas1", &sprite, &cornerPos, color);
    }

    // Fill horizontal and vertical middle areas
    float leftInset = corners[0] || corners[3] ? radius : 0;
    float rightInset = corners[1] || corners[2] ? radius : 0;
    float topInset = corners[0] || corners[1] ? radius : 0;
    float bottomInset = corners[2] || corners[3] ? radius : 0;

    SDL_FRect filler;
    filler = {rect->x + leftInset, rect->y, rect->w - leftInset - rightInset + 1, rect->h + 1};
    renderRect(&filler, color);

    filler = {rect->x, rect->y + topInset, rect->w + 1, rect->h - topInset - bottomInset + 1};
    renderRect(&filler, color);
}


void Renderer::renderRoundedImage(std::string_view imageName, const SDL_FRect* rect, float radius, const SDL_Color& backgroundColor) {
    renderImage(imageName, rect);

    //top left
    SDL_FRect cornerPos = {
        rect->x,
        rect->y - 1,
        radius,
        radius
    };
    SDL_FRect sprite = {271, 271, 256, 256};
    renderSprite("atlas2", &sprite, &cornerPos, backgroundColor);

    //top right
    cornerPos.x = rect->x + rect->w - radius;
    sprite = {5, 271, 256, 256};
    renderSprite("atlas2", &sprite, &cornerPos, backgroundColor);

    //bottom right
    sprite = {5, 5, 256, 256};
    cornerPos.y = rect->y + rect->h - radius;
    renderSprite("atlas2", &sprite, &cornerPos, backgroundColor);

    //bottom left
    cornerPos.x = rect->x;
    sprite = {271, 5, 256, 256};
    renderSprite("atlas2", &sprite, &cornerPos, backgroundColor);
}

void Renderer::renderText(
    std::string_view fontName,
    std::string_view text,
    Float2 pos,
    TEXT_ALIGNMENT textPosition,
    const SDL_Color& color,
    int wrapWidth
) {
    if (m_TextTextures.count({text, fontName}) == 0) {
        SDL_Surface* srf = TTF_RenderText_Blended_Wrapped(m_ResourceManager.getFont(fontName), text.data(), text.length(), SDL_Color{255, 255, 255, 255}, wrapWidth);
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
