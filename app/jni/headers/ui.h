#ifndef BOULDER_UI_H
#define BOULDER_UI_H
#pragma once

#include "iostream"
#include "SDL.h"
#include "SDL_ttf.h"
#include "vector"

class UIElement  {
protected:
    std::string id;
    SDL_Rect rect;
    SDL_Color black;
    SDL_Color gray;

public:
    UIElement(const std::string& id, int x, int y, int w, int h); 
    virtual ~UIElement();

    virtual void render(SDL_Renderer* renderer);
    bool isClicked(int mouseX, int mouseY);
    std::string getId();
};

class IconCard : UIElement {
private:
    SDL_Color color;
    SDL_Texture* text;
    int textW, textH;
    SDL_Texture* icon;
    SDL_Color iconColor;
    int borderRadius;

public:
    IconCard(const std::string& id, SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h, int borderRadius, SDL_Color color, const std::string text, SDL_Texture* icon, SDL_Color iconColor);
    ~IconCard();
    void render(SDL_Renderer* renderer) override;
};

class BigValueCard : UIElement {
private:
    SDL_Color color;
    SDL_Color iconColor;
    SDL_Texture* title;
    SDL_Texture* value;
    SDL_Texture* min;
    SDL_Texture* max;
    SDL_Texture* minusIcon;
    SDL_Texture* plusIcon;
    int titleW, titleH;
    int valueW, valueH;
    int minW, minH;
    int maxW, maxH;
    int borderRadius;

public:
    BigValueCard(const std::string& id, SDL_Renderer* renderer, TTF_Font* mainFont, TTF_Font* valueFont, int x, int y, int w, int h, int borderRadius, SDL_Color color, SDL_Color iconColor, SDL_Texture* plusIcon, SDL_Texture* minusIcon, const std::string& title, const std::string& value);
    ~BigValueCard();

    void render (SDL_Renderer* renderer) override;
};



void drawBackButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect);
void drawSettingsButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect);
void drawGenerateButton(SDL_Renderer* renderer, int windowW, SDL_Texture* buttonImage, SDL_Rect* imageRect) ;

void drawMainImage(SDL_Renderer* renderer, int windowH, int windowW, SDL_Texture* mainImage, SDL_Rect* mainImageRect, SDL_Texture* mainImageMask);

void drawSelectHoldMenu(SDL_Renderer* renderer, int x, int y, int w, int h, int holdRadius, int holdWidth, SDL_Texture* deleteImage);


class UIHandler {
private:
    std::vector<UIElement*> elements;
public:
    UIHandler();
    ~UIHandler();
    
    void addElement(UIElement* element);
    void render(SDL_Renderer* renderer);
    std::string handleClick(int mouseX, int mouseY);
    void removeElement(const std::string& id);
};

#endif //BOULDER_UI_H
