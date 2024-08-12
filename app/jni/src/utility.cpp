//
// Created by PvPSk on 7/18/2024.
//

#include "../headers/utility.h"

bool isWithinRadius(int x1, int y1, int x2, int y2, int radius) {
    int dx = x1 - x2;
    int dy = y1 - y2;
    return (dx * dx + dy * dy) <= (radius * radius);
}
