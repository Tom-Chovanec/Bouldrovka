//
// Created by PvPSk on 7/12/2024.
//

#ifndef BOULDER_RENDERING_H
#define BOULDER_RENDERING_H

#pragma once
#include "SDL.h"
#include "utility.h"


int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius, int width);
int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius);
void SDL_RenderFillRoundedRect(SDL_Renderer* renderer, int x, int y, int w, int h, int radius, const bool corners[4]);
void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color);
SDL_Texture* createRoundedRectMask(SDL_Renderer* renderer, int w, int h, int radius);
void renderImageWithMask(SDL_Renderer* renderer, SDL_Texture* mainImage, SDL_Rect* imageRect, SDL_Texture* mask);
void renderIconInCircle(SDL_Renderer* renderer, int x, int y, int radius, int width, int padding, SDL_Texture* icon);

#endif //BOULDER_RENDERING_H
