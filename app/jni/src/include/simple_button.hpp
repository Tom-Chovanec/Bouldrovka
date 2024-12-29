#pragma once

#include <SDL3/SDL.h>

#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class SimpleButton : public  Object {
private:
    SDL_FRect m_Rect;
    SDL_FRect m_RenderRect;
    SDL_Color m_Color;
    float m_Radius;

public:
    SimpleButton(const SDL_FRect& rect, float radius, const SDL_Color& color = {255, 255, 255, 0});

    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
};
