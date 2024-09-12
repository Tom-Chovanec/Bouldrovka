#ifndef BOULDER_UI_H
#define BOULDER_UI_H
#pragma once

#include "iostream"
#include "SDL.h"
#include "SDL_ttf.h"

class UIElement  {
protected:
    std::string id;
    SDL_Rect rect;

public:
    UIElement(const std::string& id, int x, int y, int w, int h); 
    virtual ~UIElement();

    virtual void Render(SDL_Renderer* renderer);
    bool isClicked(int mouseX, int mouseY);
};

class IconCard : UIElement {
private:
    SDL_Color color;
    SDL_Texture* text;
    int textW, textH;
    SDL_Texture* icon;
    SDL_Color iconColor;
    int borderRadius;

private:
    IconCard(const std::string& id, SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h, int borderRadius, SDL_Color color, const std::string text, SDL_Texture* icon, SDL_Color iconColor);
    ~IconCard();
    void Render(SDL_Renderer* renderer) override;
};



void drawBackButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect);
void drawSettingsButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect);
void drawGenerateButton(SDL_Renderer* renderer, int windowW, SDL_Texture* buttonImage, SDL_Rect* imageRect) ;

void drawMainImage(SDL_Renderer* renderer, int windowH, int windowW, SDL_Texture* mainImage, SDL_Rect* mainImageRect, SDL_Texture* mainImageMask);

void drawSelectHoldMenu(SDL_Renderer* renderer, int x, int y, int w, int h, int holdRadius, int holdWidth, SDL_Texture* deleteImage);
#endif //BOULDER_UI_H
