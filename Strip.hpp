#pragma once
#include "defines.hpp"

#include <Arduino.h>
#include <FastLED.h>

extern CRGB LedBuffer[HOLD_AMOUNT];

void runInitAnimation();
void showBoard(const std::array<uint8_t, HOLD_AMOUNT> holds);
