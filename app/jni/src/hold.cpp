#include "include/hold.hpp"
#include "include/colors.hpp"


SDL_FRect getHoldSpriteRectFromDifficulty(DIFFICULTY difficulty) {
    switch (difficulty) {
    case EASY:
        return {5, 419, 128, 128};
    case MEDIUM:
        return {5, 281, 128, 128};
    case HARD:
        return {5, 143, 128, 128};
    case IMPOSSIBLE:
        return {5, 5, 128, 128};
    }
}

Colors::COLOR getHoldColorFromType(HOLD_TYPE type) {
    switch (type) {
    case TOP:
        return Colors::TOP_HOLD;
    case UPPER:
        return Colors::UPPER_HOLD;
    case LOWER:
        return Colors::LOWER_HOLD;
    case START:
        return Colors::START_HOLD;
    case FOOT:
        return Colors::FOOT_HOLD;
    }
}

Hold::Hold(Float2 pos, DIFFICULTY difficulty, HOLD_TYPE type)
: m_Difficulty(difficulty), m_Type(type), m_Pos(pos) {
    m_SpriteRect = getHoldSpriteRectFromDifficulty(m_Difficulty);

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
