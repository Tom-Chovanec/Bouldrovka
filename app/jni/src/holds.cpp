//
// Created by PvPSk on 7/12/2024.
//

#include "../headers/holds.h"
#include "random"

SDL_Color solidColors[5] = {
        {255, 106, 20, 255}, // TOP
        {255, 192, 174, 255}, // UPPER
        {151, 153, 219, 255}, // LOWER
        {221, 34, 29, 255}, // START
        {158, 13, 13, 255}, // FOOT
};

//SDL_Color transparentColors[3] = {
//        {255, 0, 0, 100}, // TOP
//        {0, 255, 0, 100}, // UPPER
//        {0, 0, 255, 100}, // END
//};

Hold::Hold(int x, int y, HoldType holdtype) {
    this->x = x;
    this->y = y;
    this->type = holdtype;
    this->radius = 25;
}

void drawHolds(SDL_Renderer* renderer, const std::vector<std::unique_ptr<Hold>>& holds) {
    using namespace std;
    vector<Hold*> x[5];
    for (auto& hold : holds) {
        switch (hold->type) {
            case TOP:
                x[0].push_back(hold.get());
                break;
            case UPPER:
                x[1].push_back(hold.get());
                break;
            case LOWER:
                x[2].push_back(hold.get());
                break;
            case START:
                x[3].push_back(hold.get());
                break;
            case FOOT:
                x[4].push_back(hold.get());
                break;
            default:
                break;
        }
    }

    for (int i = 0; i < 5; i++) {
        SetRenderDrawColor(renderer, solidColors[i]);
        for (auto& y : x[i]) {
            SDL_RenderDrawCircle(renderer, y->x, y->y, y->radius, 10);
        }
    }
}

std::vector<std::unique_ptr<Hold>> getGeneratedHolds(const std::vector<std::unique_ptr<Hold>>& holds, int amount[5]) {
    std::vector<std::unique_ptr<Hold>> result;
    std::vector<std::unique_ptr<Hold>> x[5];

    for (auto& hold : holds) {
        switch (hold->type) {
            case TOP:
                x[0].push_back(std::make_unique<Hold>(*hold));
                break;
            case UPPER:
                x[1].push_back(std::make_unique<Hold>(*hold));
                break;
            case LOWER:
                x[2].push_back(std::make_unique<Hold>(*hold));
                break;
            case START:
                x[3].push_back(std::make_unique<Hold>(*hold));
                break;
            case FOOT:
                x[4].push_back(std::make_unique<Hold>(*hold));
                break;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (x[i].size() < amount[i]) continue;
        std::vector<int> indices(x[i].size());
        std::iota(indices.begin(), indices.end(), 0);
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(indices.begin(), indices.end(), g);

        for (int j = 0; j < amount[i]; ++j) {
            result.push_back(std::make_unique<Hold>(*x[i][indices[j]]));
        }
    }

    return result;
}