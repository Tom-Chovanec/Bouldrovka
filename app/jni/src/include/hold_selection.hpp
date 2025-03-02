#pragma once

#include <SDL3/SDL.h>
#include <functional>

#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class HoldSelection : public  Object {
private:
    SDL_FRect m_Rect;
    SDL_FRect m_RenderRect;
    std::function<void(void)> m_Callback;

public:
    HoldSelection(const SDL_FRect& rect, const std::function<void(void)>& callback = []{});
    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
    void handleClick(const Context& context) override;
};
