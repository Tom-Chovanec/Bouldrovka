#include "include/globals.hpp"
#include "include/hold.hpp"

DIFFICULTY g_HoldSelectionDifficulty[5] = {
    DIFFICULTY::EASY,
    DIFFICULTY::MEDIUM,
    DIFFICULTY::HARD,
    DIFFICULTY::IMPOSSIBLE,
    DIFFICULTY::MEDIUM,
};

HoldType g_selectedHoldType;
bool g_holdIsSelected = false;
bool g_deleting = false;


