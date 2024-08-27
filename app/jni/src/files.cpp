//
// Created by PvPSk on 8/27/2024.
//

#include "../headers/files.h"

void saveHoldsToFile(const std::vector<std::unique_ptr<Hold>>& Holds, const std::string& path) {
    std::ofstream file;

    file.open(path.c_str());
    if (file.is_open()) {

        for (auto& hold : Holds) {
            std::string x = std::to_string(hold->x);
            std::string y = std::to_string(hold->y);
            std::string type = std::to_string(hold->type);
            file << x << ";" << y << ";" << type << "\n";
        }
        file.close();
    } else {
        SDL_Log("file couldnt be opened");
    }
}

std::vector<std::unique_ptr<Hold>> readHoldsFromFile(const std::string& path) {
    std::vector<std::unique_ptr<Hold>> holds;
    HoldType type;
    int x;
    int y;
    std::ifstream file;
    file.open(path.c_str());
    std::string line;
    while (getline(file, line)) {
        std::vector<std::string> a;
        a = splitString(line, ';');
        type = (HoldType)std::stoi(a.back());
        a.pop_back();
        y = std::stoi(a.back());
        a.pop_back();
        x = std::stoi(a.back());
        a.pop_back();
        holds.push_back(std::make_unique<Hold>(x, y, type));
    }
    file.close();
    return holds;
}
