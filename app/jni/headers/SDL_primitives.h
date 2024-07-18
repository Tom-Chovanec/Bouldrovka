//
// Created by PvPSk on 7/12/2024.
//

#ifndef BOULDER_SDL_PRIMITIVES_H
#define BOULDER_SDL_PRIMITIVES_H

#pragma once
#include "../SDL2_image/Xcode/tvOS/SDL2.framework/Headers/SDL.h"


int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius);
int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);
void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color);

#endif //BOULDER_SDL_PRIMITIVES_H
