//
// Created by PvPSk on 7/12/2024.
//

#include "../headers/ui.h"

Button::Button(SDL_Renderer* renderer, int x, int y, int w, int h, std::string name, TTF_Font* font) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->name = name;
    this->rect = {x, y, w, h};
    SDL_Surface* textSurf = TTF_RenderText_Solid(font, name.c_str(), {0, 0, 0, 255});
    this->textTexture = SDL_CreateTextureFromSurface(renderer, textSurf);
    SDL_FreeSurface(textSurf);
}

Button::~Button() {
    SDL_DestroyTexture(textTexture);
}

void Button::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 186, 168, 152, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderCopy(renderer, textTexture, NULL, &rect);
}

bool Button::collides(int x, int y) {
    SDL_Point point = {x, y};
    if (SDL_PointInRect(&point, &rect)) {
        return true;
    } else {
        return false;
    }
}