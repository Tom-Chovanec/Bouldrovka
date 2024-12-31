#include "include/text.hpp"


Text::Text(
    Float2 pos,
    const char* fontName,
    const std::string& text,
    COLORS color,
    Renderer::TEXT_ALIGNMENT textAlignment
) :
    m_Pos(pos),
    m_FontName(fontName),
    m_Text(text),
    m_Color(color),
    m_TextAlignment(textAlignment) {
}

void Text::render(const Context& context, Renderer& renderer)  {
    renderer.renderText(m_FontName, m_Text, m_RenderPos, m_TextAlignment, getColor(m_Color, gTheme));
}

void Text::update(const Context& context) {
    m_RenderPos.x = m_Pos.x * context.windowSize.x;
    m_RenderPos.y = m_Pos.y * context.windowSize.y;
}
