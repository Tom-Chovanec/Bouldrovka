//
// Created by PvPSk on 7/18/2024.
//

#ifndef BOULDER_UTILITY_H
#define BOULDER_UTILITY_H

#pragma once
#include "SDL.h"

bool isWithinRadius(int x1, int y1, int x2, int y2, int radius);
bool isPointInRoundedRect(int x, int y, SDL_Rect rect, int radius);
#endif //BOULDER_UTILITY_H
