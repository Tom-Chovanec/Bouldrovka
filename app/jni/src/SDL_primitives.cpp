//
// Created by PvPSk on 7/12/2024.
//

#include "../headers/SDL_primitives.h"

#include <SDL.h>

int SDL_RenderDrawCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    SDL_Point points[8 * radius];  // Prepare a buffer large enough to hold all points
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

    // Render all points in a single call
    status = SDL_RenderDrawPoints(renderer, points, pointCount);

    return status;
}

int SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    int status = 0;

    // Array to hold points
    SDL_Point* points = (SDL_Point*)malloc(4 * radius * radius * sizeof(SDL_Point));
    int pointCount = 0;

    // Loop through each row of the circle
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = (int)sqrt(radius * radius - dy * dy);
        for (int i = -dx; i <= dx; i++) {
            points[pointCount++] = (SDL_Point){ x + i, y + dy };
        }
    }

    // Render all points in a single call
    status = SDL_RenderDrawPoints(renderer, points, pointCount);

    // Free the allocated memory
    free(points);

    return status;
}

void SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}



