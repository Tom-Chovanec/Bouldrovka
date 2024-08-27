//
// Created by PvPSk on 8/27/2024.
//

#ifndef BOULDER_FILES_H
#define BOULDER_FILES_H

#pragma once
#include "iostream"
#include "vector"
#include "memory"
#include "holds.h"
#include "string"
#include "fstream"
#include "utility.h"
#include "SDL.h"

void saveHoldsToFile(const std::vector<std::unique_ptr<Hold>>& Holds, const std::string& path);

std::vector<std::unique_ptr<Hold>> readHoldsFromFile(const std::string& path);

#endif //BOULDER_FILES_H
