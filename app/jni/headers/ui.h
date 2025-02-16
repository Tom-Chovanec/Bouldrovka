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
    int scroll = 0;

public:
    UIElement(const std::string& id, int x, int y, int w, int h); 
    virtual ~UIElement();

    virtual void render(SDL_Renderer* renderer);
    virtual void hover(int mouseY, int mouseX);
    bool isClicked(int mouseX, int mouseY);
    std::string getId();
    void applyScroll();
    void revertScroll();
    int getX();
    int getY();
    void setScroll(int scroll);
};

class IconCard : public UIElement {
private:
    SDL_Color color;
    SDL_Color hoverColor;
    SDL_Texture* text;
    int textW, textH;
    SDL_Texture* icon;
    SDL_Color iconColor;
    int borderRadius;
    bool drawInCircle;
    bool isHovered = false;
    TTF_Font* font;

public:
    IconCard(const std::string& id, SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h, int borderRadius, SDL_Color color, SDL_Color hoverColor, const std::string text, SDL_Texture* icon, SDL_Color iconColor, bool drawInCirlce = true);
    ~IconCard();
    void render(SDL_Renderer* renderer) override;
    void hover(int mouseX, int mouseY) override;
    void changeText(SDL_Renderer* renderer, const std::string& text);
};

class BigValueCard : public UIElement {
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

    void render(SDL_Renderer* renderer) override;
};

class Text : public UIElement {
private:
    SDL_Texture* texture;

public:
    Text(const std::string& id, SDL_Renderer* renderer, TTF_Font* font, int x, int y, const std::string& text, SDL_Color color);
    ~Text();
    
    void changeText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const std::string& text);
    void render(SDL_Renderer* renderer) override;
};

class Image : public UIElement {
private:
    SDL_Texture* texture;
    SDL_Color color;

public:
    Image(const std::string& id, SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Texture* texture, SDL_Color color);
    ~Image();
    void render(SDL_Renderer* renderer) override;
};

class Button : public UIElement {
private:
    SDL_Texture* textTexture;
    SDL_Rect textRect;
    int textW, textH;
    SDL_Color color;
    int radius;
    bool corners[4] = {true, true, true, true};

public:
    Button(const std::string& id, SDL_Renderer* renderer, TTF_Font* font, int x, int y, int w, int h, const std::string& text, SDL_Color color, SDL_Color textColor, int radius);
    ~Button();
    void render(SDL_Renderer* renderer) override;
};

void drawBackButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect);
void drawButton(SDL_Renderer* renderer, SDL_Texture* buttonImage, SDL_Rect* buttonRect);
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
    void render(SDL_Renderer* renderer, const std::vector<std::string>& ids = {});
    std::string handleClick(int mouseX, int mouseY, const std::vector<std::string>& ids = {});
    void removeElement(const std::string& id);
    void scroll(int scroll, const std::vector<std::string>& ids = {});
    void hover(int mouseX, int mouseY, const std::vector<std::string>& ids = {});
};

#endif //BOULDER_UI_H
