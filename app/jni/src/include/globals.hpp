#pragma once

#include "hold.hpp"

struct HoldType {
    DIFFICULTY difficulty;
    HOLD_TYPE type;
};

extern DIFFICULTY g_HoldSelectionDifficulty[5];

extern HoldType g_selectedHoldType;
extern bool g_holdIsSelected;
extern bool g_deleting;
