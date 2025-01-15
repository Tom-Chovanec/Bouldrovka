#pragma once

#include <SDL3/SDL.h>

namespace Colors {
    enum COLOR_THEME {
        LIGHT = 0,
        DARK,
        THEMES_COUNT,
    };

    extern COLOR_THEME gTheme;

    enum COLOR {
        WHITE = 0,
        BLACK,
        PRIMARY,
        SECONDARY,
        TEXT_LIGHT_LIGHT,
        TEXT_LIGHT_DARK,
        TEXT_DARK_LIGHT,
        TEXT_DARK_DARK,
        TOP_HOLD,
        UPPER_HOLD,
        LOWER_HOLD,
        START_HOLD,
        FOOT_HOLD,
        COLORS_COUNT,
    };

    const SDL_Color& getColor(COLOR color);
}

