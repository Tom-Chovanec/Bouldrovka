#pragma once

#include <SDL3/SDL.h>

#include "object.hpp"
#include "common.hpp"
#include "renderer.hpp"

class BoulderSelection : public  Object {
private:
    SDL_FRect m_Rect;
    SDL_FRect m_RenderRect;
    std::string m_TextureName;
    std::string m_Name;
    std::string m_Desctription;

public:
    BoulderSelection(const std::string& textureName, const SDL_FRect& rect);
    void update(const Context& context) override;

    void render(const Context& context, Renderer& renderer) override;
};
