#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "colors.hpp"
#include "object.hpp"

class Text : public Object{
private:
    Float2 m_Pos;
    Float2 m_RenderPos;
    const char* m_FontName;
    std::string m_Text;
    COLORS m_Color;
    Renderer::TEXT_ALIGNMENT m_TextAlignment;

public:
    Text(
        Float2 pos,
        const char* fontName,
        const std::string& text,
        COLORS color,
        Renderer::TEXT_ALIGNMENT textAlignment = Renderer::MID
    );

    void render(const Context& context, Renderer& renderer) override;
    void update(const Context& context) override;
};
