#include "include/hold_selection.hpp"
#include "include/colors.hpp"

HoldSelection::HoldSelection(const SDL_FRect& rect, const std::function<void(void)>& callback)
: m_Rect(rect),
  m_Callback(callback) {

}

void HoldSelection::update(const Context& context) {
    m_RenderRect = {
        m_Rect.x * context.windowSize.x,
        m_Rect.y * context.windowSize.y,
        m_Rect.w * context.windowSize.x,
    };

    // the height is ignored in favor of having proper padding
    float padding = m_RenderRect.w * 0.15;
    m_RenderRect.h = 6 * ((m_RenderRect.w - 2 * padding) + padding) + padding; 
}

void HoldSelection::render(const Context& context, Renderer& renderer) {
    renderer.renderRoundedRect(&m_RenderRect, 50, getColor(Colors::COLOR::SECONDARY), {true, false, false, true});

    float padding = m_RenderRect.w * 0.15;
    SDL_FRect holdRect = {
        m_RenderRect.x + padding,
        m_RenderRect.y + padding,
        m_RenderRect.w - 2 * padding,
        m_RenderRect.w - 2 * padding 
    };

    SDL_FRect spriteRect = {5, 281, 128, 128};

    Colors::COLOR holdColors[5] = {
        Colors::TOP_HOLD,
        Colors::UPPER_HOLD,
        Colors::LOWER_HOLD,
        Colors::START_HOLD,
        Colors::FOOT_HOLD
    };

    for (size_t i = 0; i < 5; ++i) {
        renderer.renderSprite("atlas1", &spriteRect, &holdRect, getColor(holdColors[i]));
        holdRect.y += m_RenderRect.w - padding;
    }

    spriteRect = {5, 5, 128, 128};
    renderer.renderSprite("atlas1", &spriteRect, &holdRect, getColor(Colors::BLACK));
    spriteRect = {690, 5, 128, 128};
    SDL_FRect crossRect = {
        holdRect.x + padding,
        holdRect.y + padding,
        holdRect.w - 2 * padding,
        holdRect.w - 2 * padding
    };
    renderer.renderSprite("atlas1", &spriteRect, &crossRect, getColor(Colors::BLACK));
}

void HoldSelection::handleClick(const Context& context) {

}
