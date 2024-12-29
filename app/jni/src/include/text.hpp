#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "object.hpp"

class Text : public Object{
private:
    Float2 m_Pos;
    Float2 m_RenderPos;
    const char* m_FontName;
    std::string m_Text;
    SDL_Color m_Color;

public:
    Text(
        Float2 pos,
        const char* fontName,
        const std::string& text,
        const SDL_Color& color = {255, 255, 255, 255}
    );

    void render(const Context& context, Renderer& renderer) override;
    void update(const Context& context) override;
};
