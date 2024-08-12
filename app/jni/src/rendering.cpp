//
// Created by PvPSk on 7/12/2024.
//

#pragma once
#include "../headers/rendering.h"

#include <SDL.h>



int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius, int width) {
    int offsetx, offsety, d;
    int status;

    for (int i = 0; i < width; i++) {
        offsetx = 0;
        offsety = radius - i;
        d = (radius - i) - 1;
        status = 0;

        SDL_Point points[8 * radius - i];
        int pointCount = 0;

        while (offsety >= offsetx) {
            points[pointCount++] = {x + offsetx, y + offsety};
            points[pointCount++] = {x + offsety, y + offsetx};
            points[pointCount++] = {x - offsetx, y + offsety};
            points[pointCount++] = {x - offsety, y + offsetx};
            points[pointCount++] = {x + offsetx, y - offsety};
            points[pointCount++] = {x + offsety, y - offsetx};
            points[pointCount++] = {x - offsetx, y - offsety};
            points[pointCount++] = {x - offsety, y - offsetx};

            if (d >= 2 * offsetx) {
                d -= 2 * offsetx + 1;
                offsetx += 1;
            }
            else if (d < 2 * (radius - offsety)) {
                d += 2 * offsety - 1;
                offsety -= 1;
            }
            else {
                d += 2 * (offsety - offsetx - 1);
                offsety -= 1;
                offsetx += 1;
            }
        }

        SDL_RenderDrawPoints(renderer, points, pointCount);
    }

    return 0;
}

int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    int status = 0;

    SDL_Point* points = (SDL_Point*)malloc(4 * radius * radius * sizeof(SDL_Point));
    int pointCount = 0;

    for (int dy = -radius; dy <= radius; dy++) {
        int dx = (int)sqrt(radius * radius - dy * dy);
        for (int i = -dx; i <= dx; i++) {
            points[pointCount++] = (SDL_Point){ x + i, y + dy };
        }
    }

    status = SDL_RenderDrawPoints(renderer, points, pointCount);

    free(points);

    return status;
}

void SDL_RenderFillRoundedRect(SDL_Renderer* renderer, int x, int y, int w, int h, int radius, const bool corners[4]) {
    if (corners[0] == 1) {
        SDL_RenderFillCircle(renderer, x + radius, y + radius, radius);  // Top-left
    } else {
        SDL_Rect n = { x, y, w / 2, h / 2};
        SDL_RenderFillRect(renderer, &n);
    }
    if (corners[1] == 1) {
        SDL_RenderFillCircle(renderer, x + w - radius - 1, y + radius, radius);  // Top-right
    } else {
        SDL_Rect n = { x + w / 2, y, w / 2, h / 2};
        SDL_RenderFillRect(renderer, &n);
    }
    if (corners[2] == 1) {
        SDL_RenderFillCircle(renderer, x + radius, y + h - radius - 1, radius); // Bottom-left
    } else {
        SDL_Rect n = { x , y + h / 2, w / 2, h / 2};
        SDL_RenderFillRect(renderer, &n);
    }
    if (corners[3] == 1) {
        SDL_RenderFillCircle(renderer, x + w - radius - 1, y + h - radius - 1, radius); // Bottom-right
    } else {
        SDL_Rect n = { x + w / 2, y + h / 2, w / 2, h / 2};
        SDL_RenderFillRect(renderer, &n);
    }

    SDL_Rect rects[5] = {
            {x + radius, y, w - 2 * radius, radius}, // Top
            {x + radius, y + h - radius, w - 2 * radius, radius}, // Bottom
            {x, y + radius, radius, h - 2 * radius}, // Left
            {x + w - radius, y + radius, radius, h - 2 * radius}, // Right
            {x + radius, y + radius, w - 2 * radius, h - 2 * radius} // Center
    };

    SDL_RenderFillRects(renderer, rects, 5);
}

void SDL_RenderInverseRoundedRect(SDL_Renderer *renderer, int x, int y, int w, int h, int radius) {
    int minX = x - radius;
    int maxX = x + w + radius;
    int minY = y - radius;
    int maxY = y + h + radius;
    SDL_Rect rect = {x, y, w, h};

    // Iterate over each pixel in the bounding box
    for (int i = minY; i < maxY; ++i) {
        for (int j = minX; j < maxX; ++j) {
            if (!isPointInRoundedRect(j, i, rect, radius)) {
                SDL_RenderDrawPoint(renderer, j, i);
            }
        }
    }
}

SDL_Texture* createRoundedRectMask(SDL_Renderer* renderer, int w, int h, int radius) {
    SDL_Texture* mask = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetRenderTarget(renderer, mask);
    SDL_SetRenderDrawColor(renderer, 255, 241, 233, 255);
    //SDL_RenderClear(renderer);

    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderInverseRoundedRect(renderer, 0, 0, w, h, radius);

    SDL_SetRenderTarget(renderer, nullptr);
    return mask;


}

void renderImageWithMask(SDL_Renderer* renderer, SDL_Texture* mainImage, SDL_Rect* imageRect, SDL_Texture* mask) {
    SDL_Texture* result = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, imageRect->w, imageRect->h);
    SDL_SetTextureBlendMode(result, SDL_BLENDMODE_BLEND); // blend - mod - none - none
    SDL_SetRenderTarget(renderer, result);
    SDL_SetTextureBlendMode(mask, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(mainImage, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, mainImage, NULL, NULL);
    SDL_RenderCopy(renderer, mask, NULL, NULL);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer,result, NULL, imageRect);
    SDL_DestroyTexture(result);
}

void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void renderIconInCircle(SDL_Renderer* renderer, int x, int y, int radius, int width, int padding, SDL_Texture* icon) {
    SDL_RenderDrawCircle(renderer, x, y, radius, width);
    SDL_Rect rect = {
            x - radius + width + padding * 2,
            y - radius + width + padding * 2,
            radius * 2 - width * 2 - padding * 4,
            radius * 2 - width * 2 - padding * 4
    };
    SDL_RenderCopy(renderer, icon, nullptr, &rect);
}

