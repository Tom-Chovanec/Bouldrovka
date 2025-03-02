#include "include/hold.hpp"
#include "include/colors.hpp"

Hold::Hold(Float2 pos, DIFFICULTY difficulty, HOLD_TYPE type)
: m_Difficulty(difficulty), m_Type(type), m_Pos(pos) {
    switch (m_Difficulty) {
    case EASY:
        m_SpriteRect = {5, 419, 128, 128};
        break;
    case MEDIUM:
        m_SpriteRect = {5, 281, 128, 128};
        break;
    case HARD:
        m_SpriteRect = {5, 143, 128, 128};
        break;
    case IMPOSSIBLE:
        m_SpriteRect = {5, 5, 128, 128};
        break;
    };

    switch (m_Type) {
    case TOP:
        m_Color = Colors::TOP_HOLD;
        break;
    case UPPER:
        m_Color = Colors::UPPER_HOLD;
        break;
    case LOWER:
        m_Color = Colors::LOWER_HOLD;
        break;
    case START:
        m_Color = Colors::START_HOLD;
        break;
    case FOOT:
        m_Color = Colors::FOOT_HOLD;
        break;
    }
}


void Hold::render(const Context& context, const Renderer& renderer) const {
    renderer.renderSprite("atlas1", &m_SpriteRect, &m_RenderRect, Colors::getColor(m_Color));
}

constexpr float HOLD_SIZE = 0.1;

void Hold::update(const Context& context) {
    m_RenderRect = {
        (m_Pos.x - HOLD_SIZE / 2) * context.windowSize.x,
        (m_Pos.y - HOLD_SIZE / 2) * context.windowSize.y,
        HOLD_SIZE * context.windowSize.x,
        HOLD_SIZE * context.windowSize.x,
    };
}

Float2 Hold::getPos() const {
    return m_Pos;
}
