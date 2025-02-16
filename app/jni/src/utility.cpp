//
// Created by PvPSk on 7/18/2024.
//

#include "../headers/utility.h"



bool isWithinRadius(int x1, int y1, int x2, int y2, int radius) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return (dx * dx + dy * dy) <= (radius * radius);
}

bool isWithinRadius(SDL_Point* first, SDL_Point* second, int radius) {
    int dx = first->x - second->x;
    int dy = first->y - second->y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

bool isPointInRoundedRect(int x, int y, SDL_Rect rect, int radius) {
    if ((x >= rect.x + radius && x < rect.x + rect.w - radius) ||
        (y >= rect.y + radius && y < rect.y + rect.h)) {
        return true;
    }

    int dx, dy;
    // Top-left corner
    dx = rect.x + radius - x;
    dy = rect.y + radius - y;
    if (dx * dx + dy * dy <= radius * radius) return true;

    // Top-right corner
    dx = rect.x + rect.w - radius - x;
    dy = rect.y + radius - y;
    if (dx * dx + dy * dy <= radius * radius) return true;

    // Bottom corners not needed

    return false;
}

SDL_Texture* getTextureFromText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, SDL_Color* color, int* w, int* h) {
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), *color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    TTF_SizeUTF8(font, text.c_str(), w, h);
    return textTexture;
}


SDL_Rect getScrolled(const SDL_Rect* rect, float scroll) {
    SDL_Rect scrolled = *rect;
    scrolled.y += (int)scroll;
    return scrolled;
}

int getScrolled(const int x, float scroll) {
    return x + (int)scroll;
}

SDL_Texture* changeColorOfTexture(SDL_Texture* texture, SDL_Color* color) {
    if (SDL_SetTextureColorMod(texture, color->r, color->g, color->b) != 0) {
        SDL_Log("Failed to modulate texture color: %s", SDL_GetError());
        return nullptr;
    }
    return texture;
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::string::size_type start = 0;
    std::string::size_type end;

    while ((end = str.find(delimiter, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    // Add the last part of the string
    result.push_back(str.substr(start));

    return result;
}

