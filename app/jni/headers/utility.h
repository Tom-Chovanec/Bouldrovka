//
// Created by PvPSk on 7/18/2024.
//

#ifndef BOULDER_UTILITY_H
#define BOULDER_UTILITY_H

#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "iostream"

bool isWithinRadius(int x1, int y1, int x2, int y2, int radius);
bool isPointInRoundedRect(int x, int y, SDL_Rect rect, int radius);

struct textureWithDimensions {
    SDL_Texture * texture;
    int w;
    int h;
};

SDL_Texture* getTextureFromText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color* color, int* w, int* h);
#endif //BOULDER_UTILITY_H
