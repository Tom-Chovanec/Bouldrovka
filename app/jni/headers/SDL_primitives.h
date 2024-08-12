//
// Created by PvPSk on 7/12/2024.
//

#ifndef BOULDER_SDL_PRIMITIVES_H
#define BOULDER_SDL_PRIMITIVES_H

#pragma once
#include "../SDL2_image/Xcode/tvOS/SDL2.framework/Headers/SDL.h"


bool isPointInRoundedRect(int x, int y, SDL_Rect rect, int radius);
int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius, int width);
int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);
void SDL_RenderFillRoundedRect(SDL_Renderer* renderer, int x, int y, int w, int h, int radius, const bool corners[4]);
void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color);
SDL_Texture* createRoundedRectMask(SDL_Renderer* renderer, int w, int h, int radius);
void renderImageWithMask(SDL_Renderer* renderer, SDL_Texture* mainImage, SDL_Rect* imageRect, SDL_Texture* mask);

#endif //BOULDER_SDL_PRIMITIVES_H
