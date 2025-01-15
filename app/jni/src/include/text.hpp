#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <string_view>

#include "colors.hpp"
#include "object.hpp"

class Text : public Object{
private:
    Float2 m_Pos;
    Float2 m_RenderPos;
    std::string_view m_FontName;
    std::string m_Text;
    Colors::COLOR m_Color;
    Renderer::TEXT_ALIGNMENT m_TextAlignment;

public:
    Text(
        Float2 pos,
        std::string_view fontName,
        std::string_view text,
        Colors::COLOR color,
        Renderer::TEXT_ALIGNMENT textAlignment = Renderer::MID
    );

    void render(const Context& context, Renderer& renderer) override;
    void update(const Context& context) override;
};
