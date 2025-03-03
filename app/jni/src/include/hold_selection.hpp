#pragma once

#include <SDL3/SDL.h>
#include <functional>

#include "hold.hpp"
#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class HoldSelection : public  Object {
private:
    SDL_FRect m_Rect;
    SDL_FRect m_RenderRect;

    float m_Padding = m_RenderRect.w * 0.15;

    SDL_FRect m_HoldRects[6];
    std::function<void(void)> m_Callback;

    static HOLD_TYPE m_HoldTypes[5];

public:
    HoldSelection(const SDL_FRect& rect, const std::function<void(void)>& callback = []{});
    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
    void handleClick(const Context& context) override;
};
