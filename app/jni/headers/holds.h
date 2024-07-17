//
// Created by PvPSk on 7/12/2024.
//

#ifndef BOULDER_HOLDS_H
#define BOULDER_HOLDS_H

#pragma once
#include "../SDL2_image/Xcode/tvOS/SDL2.framework/Headers/SDL.h"
#include "SDL_primitives.h"
#include "vector"
#include "memory"

enum HoldType {
    NONE = 0,
    DELETE,
    START,
    MIDDLE,
    END,
};

struct holdPixels {
    SDL_Color solidColor;
    SDL_Color transparentColor;
    std::vector<SDL_Point> solidPoints;
    std::vector<SDL_Point> transparentPoints;
    int solidAmount;
    int transparentAmount;
};

class Hold {
public:
    Hold(int x, int y, HoldType holdtype);
    holdPixels getHoldPixels();
    HoldType type;
private:
    SDL_Color color = {0, 0, 0, 0};
    SDL_Color transparentColor = {0, 0, 0, 0};
    int x = 0;
    int y = 0;
    int radius = 0;
};

void drawHolds(SDL_Renderer* renderer, const std::vector<std::unique_ptr<Hold>>& holds);
#endif //BOULDER_HOLDS_H

