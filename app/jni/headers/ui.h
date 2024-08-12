//
// Created by PvPSk on 7/12/2024.
//

#ifndef BOULDER_UI_H
#define BOULDER_UI_H
#pragma once
#include "iostream"
#include "SDL.h"
#include "SDL_ttf.h"

void drawBackButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect);
void drawSettingsButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect);
void drawGenerateButton(SDL_Renderer* renderer, int windowW, SDL_Texture* buttonImage, SDL_Rect* imageRect) ;

void drawMainImage(SDL_Renderer* renderer, int windowH, int windowW, SDL_Texture* mainImage, SDL_Rect* mainImageRect, SDL_Texture* mainImageMask);

void drawSelectHoldMenu(SDL_Renderer* renderer, int x, int y, int w, int h, int holdRadius, int holdWidth, SDL_Texture* deleteImage);
#endif //BOULDER_UI_H
