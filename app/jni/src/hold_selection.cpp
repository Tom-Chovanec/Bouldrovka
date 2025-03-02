#include "include/hold_selection.hpp"

HoldSelection::HoldSelection(const SDL_FRect& rect, const std::function<void(void)>& callback)
: m_Rect(rect),
  m_Callback(callback) {

}

void HoldSelection::update(const Context& context) {
    m_RenderRect = {
        m_Rect.x * context.windowSize.x,
        m_Rect.y * context.windowSize.y,
        m_Rect.w * context.windowSize.x,
        m_Rect.h * context.windowSize.y,
    };
}

void HoldSelection::render(const Context& context, Renderer& renderer) {

}

void HoldSelection::handleClick(const Context& context) {

}
