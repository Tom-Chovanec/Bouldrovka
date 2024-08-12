//
// Created by PvPSk on 7/12/2024.
//

#include "../headers/ui.h"
#include "../headers/rendering.h"
#include "../headers/holds.h"

void drawBackButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect) {
    SDL_RenderCopy(renderer, buttonImage, nullptr, buttonRect);
}

void drawSettingsButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect) {
    SDL_RenderCopy(renderer, buttonImage, nullptr, buttonRect);
}

void drawGenerateButton(SDL_Renderer* renderer, int windowW, SDL_Texture* buttonImage, SDL_Rect* imageRect) {
    bool corners[4] = {true, false, true, false};
    SDL_RenderFillRoundedRect(renderer, windowW- 170, 150, 170, 130, 40, corners);
    SDL_RenderCopy(renderer, buttonImage, nullptr, imageRect);
}

void drawMainImage(SDL_Renderer* renderer, int windowH, int windowW, SDL_Texture* mainImage, SDL_Rect* mainImageRect, SDL_Texture* mainImageMask) {
    SDL_SetRenderDrawColor(renderer, 255, 241, 233, 255);
    bool corners[4] = {true, true, false, false};
    SDL_RenderFillRoundedRect(renderer, 0, windowH - mainImageRect->h - 35, mainImageRect->w,
                              mainImageRect->h + 35, 50, corners); // image background

    if (mainImage != nullptr) {
        renderImageWithMask(renderer, mainImage, mainImageRect, mainImageMask); // render main image
    }
}

void drawSelectHoldMenu(SDL_Renderer* renderer, int x, int y, int w, int h, int holdRadius, int holdWidth, SDL_Texture* deleteImage) {
    bool corners[4] = {true, false, true, false};
    SDL_RenderFillRoundedRect(renderer, x, y, w, h, 50, corners);
    for(int i = 0; i < 5; i++) {
        SetRenderDrawColor(renderer, solidColors[i]);
        SDL_RenderDrawCircle(renderer, x + w / 2, y + 30 + holdRadius + (30 + 76) * i, holdRadius, holdWidth);
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawCircle(renderer, x + w / 2, y + 30 + holdRadius + (30 + 76) * 5, holdRadius, holdWidth);
    SDL_Rect rect = {
            x + w / 2 - 18,
            y + h - w / 2 - 25,
            36,
           36
    };
    SDL_RenderCopy(renderer, deleteImage, nullptr, &rect);
}
