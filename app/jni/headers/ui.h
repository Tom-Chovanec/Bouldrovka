//
// Created by PvPSk on 7/12/2024.
//

#ifndef BOULDER_UI_H
#define BOULDER_UI_H
#pragma once
#include "iostream"
#include "SDL.h"
#include "SDL_ttf.h"

class Button {
public:
    Button(SDL_Renderer* renderer, int x, int y, int w, int h, std::string name, TTF_Font* font);
    ~Button();
    void draw(SDL_Renderer* renderer);
    bool collides(int x, int y);
    std::string name;
private:
    int x;
    int y;
    int w;
    int h;
    SDL_Rect rect;
    SDL_Texture* textTexture;
};
#endif //BOULDER_UI_H
