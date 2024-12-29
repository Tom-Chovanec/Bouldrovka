#include "include/text.hpp"


Text::Text(
    Float2 pos,
    const char* fontName,
    const std::string& text,
    const SDL_Color& color
) :
    m_Pos(pos),
    m_FontName(fontName),
    m_Text(text),
    m_Color(color) {
}

void Text::render(const Context& context, Renderer& renderer)  {
    renderer.renderText(m_FontName, m_Text, m_RenderPos, Renderer::MID, m_Color);
}

void Text::update(const Context& context) {
    m_RenderPos.x = m_Pos.x * context.windowSize.x;
    m_RenderPos.y = m_Pos.y * context.windowSize.y;
}
