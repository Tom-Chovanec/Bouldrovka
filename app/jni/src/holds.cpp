//
// Created by PvPSk on 7/12/2024.
//

#include "../headers/holds.h"

Hold::Hold(int x, int y, HoldType holdtype) {
    this->x = x;
    this->y = y;
    this->type = holdtype;
    this->radius = 25;
    switch (type) {
        case NONE:
            break;
        case START:
            color = {255, 107, 108, 255};
            break;
        case MIDDLE:
            color = {91, 95, 151, 255};
            break;
        case END:
            color = {255, 193, 69, 255};
            break;
    }
    transparentColor = {color.r, color.g, color.b, 100};
}

holdPixels Hold::getHoldPixels() {
    int offsetx, offsety, d;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;

    std::vector<SDL_Point> solidPoints;
    int solidPointCount = 0;

    while (offsety >= offsetx) {
        solidPoints[solidPointCount++] = {x + offsetx, y + offsety};
        solidPoints[solidPointCount++] = {x + offsety, y + offsetx};
        solidPoints[solidPointCount++] = {x - offsetx, y + offsety};
        solidPoints[solidPointCount++] = {x - offsety, y + offsetx};
        solidPoints[solidPointCount++] = {x + offsetx, y - offsety};
        solidPoints[solidPointCount++] = {x + offsety, y - offsetx};
        solidPoints[solidPointCount++] = {x - offsetx, y - offsety};
        solidPoints[solidPointCount++] = {x - offsety, y - offsetx};

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


    std::vector<SDL_Point> transparentPoints;
    int transparentPointCount = 0;

    // Loop through each row of the circle
    for (int dy = -radius; dy <= radius; dy++) {
        int dx = (int)sqrt(radius * radius - dy * dy);
        for (int i = -dx; i <= dx; i++) {
            transparentPoints[transparentPointCount++] = (SDL_Point){ x + i, y + dy };
        }
    }

    holdPixels out = {color, transparentColor, solidPoints, transparentPoints, solidPointCount, transparentPointCount};


    return out;
}

void drawHolds(SDL_Renderer* renderer, const std::vector<std::unique_ptr<Hold>>& holds) {
    holdPixels pixels[3];
    for (auto& hold : holds) {
        auto x = hold->getHoldPixels();
       if (hold->type == START) {
            pixels[0].solidPoints.insert(pixels[0].solidPoints.end(), x.solidPoints.begin(), x.solidPoints.end());
            pixels[0].solidAmount += x.solidAmount;
       }
    }
    pixels[0].solidColor = {255, 0, 0, 255};
    int i = 0;
    SDL_SetRenderDrawColor(renderer, pixels[i].solidColor.r, pixels[i].solidColor.g, pixels[i].solidColor.b, pixels[i].solidColor.a);
    SDL_RenderDrawPoints(renderer, pixels[i].solidPoints.data(), pixels[i].solidAmount);
    //SDL_SetRenderDrawColor(renderer, Pixels.transparentColor.r, Pixels.transparentColor.g, Pixels.transparentColor.b, Pixels.transparentColor.a);
    //SDL_RenderDrawPoints(renderer, Pixels.transparentPoints, Pixels.transparentAmount);
}
