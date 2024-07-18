//
// Created by PvPSk on 7/12/2024.
//

#include "../headers/holds.h"

SDL_Color solidColors[3] = {
        {255, 0, 0, 255}, // START
        {0, 255, 0, 255}, // MIDDLE
        {0, 0, 255, 255}, // END
};
SDL_Color transparentColors[3] = {
        {255, 0, 0, 100}, // START
        {0, 255, 0, 100}, // MIDDLE
        {0, 0, 255, 100}, // END
};

Hold::Hold(int x, int y, HoldType holdtype) {
    this->x = x;
    this->y = y;
    this->type = holdtype;
    this->radius = 25;
}

void drawHolds(SDL_Renderer* renderer, const std::vector<std::unique_ptr<Hold>>& holds) {
    using namespace std;
    vector<Hold*> x[3];
    for (auto& hold : holds) {
        if (hold->type == START) {
            x[0].push_back(hold.get());
        } else if (hold->type == MIDDLE) {
            x[1].push_back(hold.get());
        } else if (hold->type == END) {
            x[2].push_back(hold.get());
        }
    }
    SetRenderDrawColor(renderer, solidColors[0]);
    for (auto& y : x[0]) {
        SDL_RenderDrawCircle(renderer, y->x, y->y, y->radius);
    }
    SetRenderDrawColor(renderer, transparentColors[0]);
    for (auto& y : x[0]) {
        SDL_RenderFillCircle(renderer, y->x, y->y, y->radius);
    }

    SetRenderDrawColor(renderer, solidColors[1]);
    for (auto& y : x[1]) {
        SDL_RenderDrawCircle(renderer, y->x, y->y, y->radius);
    }
    SetRenderDrawColor(renderer, transparentColors[1]);
    for (auto& y : x[1]) {
        SDL_RenderFillCircle(renderer, y->x, y->y, y->radius);
    }

    SetRenderDrawColor(renderer, solidColors[2]);
    for (auto& y : x[2]) {
        SDL_RenderDrawCircle(renderer, y->x, y->y, y->radius);
    }
    SetRenderDrawColor(renderer, transparentColors[2]);
    for (auto& y : x[2]) {
        SDL_RenderFillCircle(renderer, y->x, y->y, y->radius);
    }
}

