#pragma once

#include "colors.hpp"
#include "common.hpp"
#include "renderer.hpp"

enum DIFFICULTY {
    EASY = 0,
    MEDIUM,
    HARD,
    IMPOSSIBLE,
};

enum HOLD_TYPE {
    TOP = 0,
    UPPER,
    LOWER,
    START,
    FOOT
};

class Hold {
private:
    DIFFICULTY m_Difficulty;
    HOLD_TYPE m_Type;
    Float2 m_Pos;
    SDL_FRect m_RenderRect;

    SDL_FRect m_SpriteRect;
    Colors::COLOR m_Color;

public:
    Hold(Float2 pos, DIFFICULTY difficulty, HOLD_TYPE type);

    void render(const Context& context, const Renderer& renderer) const;
    void update(const Context& context);

    Float2 getPos() const;
};
