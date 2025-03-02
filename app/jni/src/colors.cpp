#include "include/colors.hpp"
#include <array>

namespace Colors {
    std::array<std::array<SDL_Color, COLORS_COUNT>, THEMES_COUNT> Colors = {
        // ---- LIGHT ----
        std::array<SDL_Color, COLORS_COUNT> {
            SDL_Color{255, 255, 255, 255},      // WHITE
            SDL_Color{0, 0, 0, 255},            // BLACK
            SDL_Color{255, 106, 20, 255},       // PRIMARY
            SDL_Color{255, 241, 233, 255},      // SECONDARY
            SDL_Color{255, 255, 255, 255},      // TEXT_LIGHT_LIGHT
            SDL_Color{255, 255, 255, 255},      // TEXT_LIGHT_DARK
            SDL_Color{24, 24, 24, 255},         // TEXT_DARK_LIGHT
            SDL_Color{24, 24, 24, 255},         // TEXT_DARK_DARK
            SDL_Color{255, 106, 20, 255},       // TOP HOLD
            SDL_Color{255, 192, 174, 255},      // UPPER HOLD
            SDL_Color{151, 153, 219, 255},      // LOWER HOLD
            SDL_Color{221, 43, 29, 255},        // START HOLD
            SDL_Color{193, 17, 17, 255},        // FOOT HOLD
        },

        // ---- DARK ----
        std::array<SDL_Color, COLORS_COUNT> {
            SDL_Color{24, 24, 24, 255},         // WHITE
            SDL_Color{255, 255, 255, 255},      // BLACK
            SDL_Color{255, 106, 20, 255},       // PRIMARY
            SDL_Color{42, 42, 42, 255},         // SECONDARY
            SDL_Color{255, 255, 255, 255},      // TEXT_LIGHT_LIGHT
            SDL_Color{24, 24, 24, 255},         // TEXT_LIGHT_DARK
            SDL_Color{255, 255, 255, 255},      // TEXT_DARK_LIGHT
            SDL_Color{24, 24, 24, 255},         // TEXT_DARK_DARK
            SDL_Color{255, 106, 20, 255},       // TOP HOLD
            SDL_Color{255, 192, 174, 255},      // UPPER HOLD
            SDL_Color{151, 153, 219, 255},      // LOWER HOLD
            SDL_Color{221, 43, 29, 255},        // START HOLD
            SDL_Color{193, 17, 17, 255},        // FOOT HOLD
        }
    };

    COLOR_THEME gTheme = DARK;

    const SDL_Color& getColor(COLOR color) {
        return Colors[gTheme][color];
    }
}

