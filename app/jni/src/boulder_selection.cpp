#include "include/boulder_selection.hpp"
#include "include/colors.hpp"

BoulderSelection::BoulderSelection(std::string_view textureName, const SDL_FRect& rect, const std::function<void(void)>& callback) : m_TextureName(textureName), m_Rect(rect), m_Callback(callback) {
}

void BoulderSelection::update(const Context& context) {
    m_RenderRect = {
        m_Rect.x * context.windowSize.x,
        m_Rect.y * context.windowSize.y,
        m_Rect.w * context.windowSize.x,
        m_Rect.h * context.windowSize.y,
    };
}

void BoulderSelection::render(const Context& context, Renderer& renderer) {
    SDL_FRect imageRect = {
        m_RenderRect.x,
        m_RenderRect.y,
        m_RenderRect.w / 2,
        m_RenderRect.h
    };

    if (m_TextureName == "") {
        renderer.renderRoundedRect(&imageRect, 40, getColor(Colors::COLOR::PRIMARY));
        SDL_FRect iconSpriteRect = { 419, 143, 128, 128 };
        SDL_FRect iconRect{
            imageRect.x + imageRect.w / 2 - 64.0f,
            imageRect.y + imageRect.h / 2 - 64.0f,
            128,
            128
        };
        renderer.renderSprite("atlas1", &iconSpriteRect, &iconRect, getColor(Colors::COLOR::WHITE));
        return;
    }

    renderer.renderRoundedImage(m_TextureName, &imageRect, 40, getColor(Colors::COLOR::WHITE));
    SDL_FRect textRect = {
        m_RenderRect.x + m_RenderRect.w * (1.0f / 3.0f),
        m_RenderRect.y + m_RenderRect.h / 10,
        m_RenderRect.w * (2.0f / 3.0f),
        m_RenderRect.h * (2.0f / 3.0f)
    };
    renderer.renderRoundedRect(&textRect, 40, getColor(Colors::COLOR::SECONDARY));

    Float2 textPos = {
        textRect.x + (textRect.w / 15),
        textRect.y + (textRect.w / 15)
    };
    renderer.renderText("regular4", "Drevený boulder", textPos, Renderer::TL, getColor(Colors::COLOR::TEXT_DARK_LIGHT));

    textPos.y = textRect.y + 2.5 * (textRect.w / 15) + context.windowSize.x * 0.04;
    renderer.renderText("regular2", "Popis jada jada jada", textPos, Renderer::TL, getColor(Colors::COLOR::TEXT_DARK_LIGHT));
} 

void BoulderSelection::handleClick(const Context& context) {
    if (SDL_PointInRectFloat(&context.mousePos, &m_RenderRect)) {
        m_Callback();
    }
}
