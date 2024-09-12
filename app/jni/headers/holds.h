//
// Created by PvPSk on 7/12/2024.
//

#ifndef BOULDER_HOLDS_H
#define BOULDER_HOLDS_H

#pragma once
#include "rendering.h"
#include "SDL.h"
#include "vector"
#include "memory"

extern SDL_Color solidColors[5];

enum HoldType {
    TOP,
    UPPER,
    LOWER,
    START,
    FOOT,
};

struct twoNum {
    int a;
    int b;
};

class Hold {
public:
    Hold(int x, int y, HoldType holdtype);
    HoldType type;
    int x = 0;
    int y = 0;
    int radius = 0;
};

void drawHolds(SDL_Renderer* renderer, const std::vector<std::unique_ptr<Hold>>& holds);

std::vector<std::unique_ptr<Hold>> getGeneratedHolds(const std::vector<std::unique_ptr<Hold>>& holds, twoNum amount[5]);

#endif //BOULDER_HOLDS_H

