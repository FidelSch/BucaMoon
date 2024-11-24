#pragma once
#include <Arduino.h>
#include <array>
#include "defines.hpp"
#include "Hold.hpp"

void runInitAnimation(void);
void showBoard(const std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> holds, const bool showMoveBeta = false);
void showBoard(void);

