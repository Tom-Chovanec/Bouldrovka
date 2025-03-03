#include "include/hold_selection.hpp"
#include "include/colors.hpp"
#include "include/common.hpp"
#include "include/globals.hpp"
#include "include/hold.hpp"
#include <SDL3/SDL_rect.h>

HoldSelection::HoldSelection(const SDL_FRect& rect, const std::function<void(void)>& callback)
: m_Rect(rect),
  m_Callback(callback) {

}

void HoldSelection::update(const Context& context) {
    m_RenderRect = {
        m_Rect.x * context.windowSize.x,
        m_Rect.y * context.windowSize.y,
        m_Rect.w * context.windowSize.x,
    };

    m_Padding = m_RenderRect.w * 0.15;

    for (size_t i = 0; i < 6; ++i) {
        m_HoldRects[i] = {
            m_RenderRect.x + m_Padding,
            m_RenderRect.y + m_Padding + i * (m_RenderRect.w - m_Padding),
            m_RenderRect.w - 2 * m_Padding,
            m_RenderRect.w - 2 * m_Padding 
        };
    }

    // the height is ignored in favor of having proper padding
    m_RenderRect.h = 6 * ((m_RenderRect.w - 2 * m_Padding) + m_Padding) + m_Padding; 
}

HOLD_TYPE HoldSelection::m_HoldTypes[5] = {
    HOLD_TYPE::TOP,
    HOLD_TYPE::UPPER,
    HOLD_TYPE::LOWER,
    HOLD_TYPE::START,
    HOLD_TYPE::FOOT
};

void HoldSelection::render(const Context& context, Renderer& renderer) {
    renderer.renderRoundedRect(&m_RenderRect, 50, getColor(Colors::COLOR::SECONDARY), {true, false, false, true});

    SDL_FRect spriteRect;

    for (size_t i = 0; i < 5; ++i) {
        spriteRect = getHoldSpriteRectFromDifficulty(g_HoldSelectionDifficulty[i]);

        if (g_selectedHoldType.type == m_HoldTypes[i] && g_holdIsSelected) spriteRect.x += 138;

        renderer.renderSprite("atlas1", &spriteRect, &m_HoldRects[i], getColor(getHoldColorFromType(m_HoldTypes[i])));
    }

    spriteRect = {5, 5, 128, 128};
    renderer.renderSprite("atlas1", &spriteRect, &m_HoldRects[5], getColor(Colors::BLACK));
    spriteRect = {690, 5, 128, 128};
    SDL_FRect crossRect = {
        m_HoldRects[5].x + m_Padding,
        m_HoldRects[5].y + m_Padding,
        m_HoldRects[5].w - 2 * m_Padding,
        m_HoldRects[5].w - 2 * m_Padding
    };
    renderer.renderSprite("atlas1", &spriteRect, &crossRect, getColor(Colors::BLACK));
}

void HoldSelection::handleClick(const Context& context) {
    if (SDL_PointInRectFloat(&context.mousePos, &m_HoldRects[0])) {
        g_selectedHoldType = {
            g_HoldSelectionDifficulty[0],
            HOLD_TYPE::TOP
        };
        g_holdIsSelected = true;
    } else if (SDL_PointInRectFloat(&context.mousePos, &m_HoldRects[1])) {
        g_selectedHoldType = {
            g_HoldSelectionDifficulty[1],
            HOLD_TYPE::UPPER
        };
        g_holdIsSelected = true;
    } else if (SDL_PointInRectFloat(&context.mousePos, &m_HoldRects[2])) {
        g_selectedHoldType = {
            g_HoldSelectionDifficulty[2],
            HOLD_TYPE::LOWER
        };
        g_holdIsSelected = true;
    } else if (SDL_PointInRectFloat(&context.mousePos, &m_HoldRects[3])) {
        g_selectedHoldType = {
            g_HoldSelectionDifficulty[3],
            HOLD_TYPE::START
        };
        g_holdIsSelected = true;
    } else if (SDL_PointInRectFloat(&context.mousePos, &m_HoldRects[4])) {
        g_selectedHoldType = {
            g_HoldSelectionDifficulty[1],
            HOLD_TYPE::FOOT
        };
        g_holdIsSelected = true;
    } else if (SDL_PointInRectFloat(&context.mousePos, &m_HoldRects[5])) {
        g_deleting = true;
        g_holdIsSelected = false;
    }





}
