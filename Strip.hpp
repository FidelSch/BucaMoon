#pragma once
#include <Arduino.h>
#include "defines.hpp"

void runInitAnimation();
void showBoard(const std::array<uint8_t, HOLD_AMOUNT> holds);

