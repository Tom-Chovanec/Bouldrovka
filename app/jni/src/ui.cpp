//
// Created by PvPSk on 7/12/2024.
//

#include "../headers/ui.h"
#include "../headers/rendering.h"
#include "../headers/holds.h"

UIElement::UIElement(const std::string& id, int x, int y, int w, int h) 
    : rect({x, y, w, h}) {}

bool UIElement::isClicked(int mouseX, int mouseY) {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

IconCard::IconCard(const std::string& id, SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h, int borderRadius, SDL_Color color, const std::string text, SDL_Texture* icon, SDL_Color iconColor) 
    : UIElement(id, x, y, w, h), color(color), icon(icon), iconColor(iconColor), borderRadius(borderRadius) {
    this->text = getTextureFromText(renderer, font, text, &color, &textW, &textH);
}

IconCard::~IconCard() {
    SDL_DestroyTexture(icon);
    SDL_DestroyTexture(text);
}

void IconCard::Render(SDL_Renderer* renderer) {
    bool corners[4] = {true, true, true, true};

    // scuffed drop shadow
    SDL_SetRenderDrawColor(renderer, 183, 174, 169, 255);
    SDL_Rect shadowRect = {
            rect.x - 5,
            rect.y + 5,
            rect.w,
            rect.h
    };
    SDL_RenderFillRoundedRect(renderer, shadowRect.x, shadowRect.y, shadowRect.w, shadowRect.h, borderRadius, corners);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRoundedRect(renderer, rect.x, rect.y, rect.w, rect.h, borderRadius, corners);
    SDL_Rect textRect = {
            rect.x + 40,
            rect.y + rect.h / 2 - textH / 2,
            textW,
            textH,
    };
    SDL_RenderCopy(renderer, text, nullptr, &textRect);

    if (icon != nullptr) {
        SDL_SetRenderDrawColor(renderer, iconColor);
        renderIconInCircle(renderer, rect.x + rect.w - 100, rect.y + rect.h / 2, borderRadius, 10, 5, icon);
    }
}

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
    if (mainImage != nullptr) {
        renderImageWithMask(renderer, mainImage, mainImageRect, mainImageMask); // render main image
    }
}

void drawSelectHoldMenu(SDL_Renderer* renderer, int x, int y, int w, int h, int holdRadius, int holdWidth, SDL_Texture* deleteImage) {
    bool corners[4] = {true, false, true, false};
    SDL_RenderFillRoundedRect(renderer, x, y, w, h, 50, corners);
    for(int i = 0; i < 5; i++) {
        SDL_SetRenderDrawColor(renderer, solidColors[i]);
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
