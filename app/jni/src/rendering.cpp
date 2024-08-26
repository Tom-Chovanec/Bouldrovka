//
// Created by PvPSk on 7/12/2024.
//

#include "../headers/rendering.h"


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

    auto* points = (SDL_Point*)malloc(4 * radius * radius * sizeof(SDL_Point));
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

void SDL_RenderFillRoundedRect(SDL_Renderer* renderer, SDL_Rect* rect, int radius, const bool corners[4]) {
    SDL_RenderFillRoundedRect(renderer, rect->x, rect->y, rect->w, rect->h, radius, corners);
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
    SDL_RenderCopy(renderer, mainImage, nullptr, nullptr);
    SDL_RenderCopy(renderer, mask, nullptr, nullptr);
    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer,result, nullptr, imageRect);
    SDL_DestroyTexture(result);
}

void SDL_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
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

void drawCardWithIcon(SDL_Renderer* renderer, SDL_Rect* rect, int radius, textureWithDimensions text, SDL_Color* iconColor, SDL_Texture* icon) {
    bool corners[4] = {true, true, true, true};

    // scuffed drop shadow
    SDL_SetRenderDrawColor(renderer, 183, 174, 169, 255);
    SDL_Rect shadowRect = {
            rect->x - 5,
            rect->y + 5,
            rect->w,
            rect->h
    };
    SDL_RenderFillRoundedRect(renderer, shadowRect.x, shadowRect.y, shadowRect.w, shadowRect.h, radius, corners);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRoundedRect(renderer, rect->x, rect->y, rect->w, rect->h, radius, corners);
    SDL_Rect textRect = {
            rect->x + 40,
            rect->y + rect->h / 2 - text.h / 2,
            text.w,
            text.h,
    };
    SDL_RenderCopy(renderer, text.texture, nullptr, &textRect);

    if (icon != nullptr) {
        SDL_SetRenderDrawColor(renderer, *iconColor);
        renderIconInCircle(renderer, rect->x + rect->w - 100, rect->y + rect->h / 2, radius, 10, 5, icon);
    }
}

void drawCardWithValue(SDL_Renderer* renderer, SDL_Rect* rect, int radius, textureWithDimensions title, textureWithDimensions firstValue, textureWithDimensions secondValue, textureWithDimensions value, SDL_Texture* minus, SDL_Texture* plus, SDL_Color* iconColor, SDL_Rect hitboxes[4]) {
    bool corners[4] = {true, true, true, true};

    // scuffed drop shadow
    SDL_SetRenderDrawColor(renderer, 183, 174, 169, 255);
    SDL_Rect shadowRect = {
            rect->x - 5,
            rect->y + 5,
            rect->w,
            rect->h
    };
    SDL_RenderFillRoundedRect(renderer, shadowRect.x, shadowRect.y, shadowRect.w, shadowRect.h, radius, corners);

    //base
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRoundedRect(renderer, rect->x, rect->y, rect->w, rect->h, radius, corners);

    //title
    SDL_Rect titleRect = {
            rect->x + 40,
            rect->y + 40 ,
            title.w,
            title.h,
    };
    SDL_RenderCopy(renderer, title.texture, nullptr, &titleRect);

    //bar
    SDL_Rect bar = {
            rect->x + 40,
            rect->y + title.h + 2 * 40,
            rect->w - 2 * 40,
            2,
    };
    SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
    SDL_RenderFillRect(renderer, &bar);

    //first value
    SDL_Rect firstValueRect = {
            rect->x + 40,
            rect->y + title.h + bar.h + 3 * 40,
            firstValue.w,
            firstValue.h,
    };
    SDL_RenderCopy(renderer, firstValue.texture, nullptr, &firstValueRect);

    //second value
    SDL_Rect secondValueRect = {
            rect->x + 40,
            rect->y + title.h + firstValue.h + bar.h + 4 * 40,
            secondValue.w,
            secondValue.h,
    };
    SDL_RenderCopy(renderer, secondValue.texture, nullptr, &secondValueRect);

    //changing value
    SDL_Rect valueRect = {
            rect->x + rect->w - value.w - 80,
            titleRect.y + title.h - value.h,
            value.w,
            value.h,

    };
    SDL_RenderCopy(renderer, value.texture, nullptr, &valueRect);

    //icons
    int iconR = 38;
    SDL_Rect iconRects[4];
    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            iconRects[x * 2 + y].x = rect->x + rect->w - iconR - 40 * (x + 1) - 80 * x;
            if (y == 0) iconRects[x * 2 + y].y = firstValueRect.y + firstValue.h / 2;
            else iconRects[x * 2 + y].y = secondValueRect.y + secondValue.h / 2;
            iconRects[x * 2 + y].w = iconR * 2;
            iconRects[x * 2 + y].h = iconR * 2;
        }
    }
    for (int i = 0; i < 4; i++) {
        hitboxes[i] = iconRects[i];
    }

    SDL_Texture* icons[4] ={
           plus, plus, minus, minus,
    };
    SDL_SetRenderDrawColor(renderer, *iconColor);
    for (int i = 0; i < 4; i++) {
        renderIconInCircle(renderer, iconRects[i].x, iconRects[i].y, iconR, 10, 5, changeColorOfTexture(icons[i], iconColor));
    }

}

SDL_Texture* getDarkenImage(SDL_Renderer* renderer,  SDL_Texture* texture, Uint8 alpha) {
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    SDL_Texture* darkenedTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetTextureBlendMode(darkenedTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, darkenedTexture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
    SDL_Rect rect = {0, 0, width, height};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderTarget(renderer, nullptr);
    return darkenedTexture;
}
