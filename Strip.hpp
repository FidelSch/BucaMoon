#pragma once
#include <Arduino.h>
#include "defines.hpp"
#include <FastLED.h>

extern CRGB LedBuffer[HOLD_AMOUNT];

void initAnimation(void *_null = nullptr);
void showBoard(const std::array<uint8_t, HOLD_AMOUNT> holds);

