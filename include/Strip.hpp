#pragma once
#include <array>
#include "defines.hpp"
#include "Hold.hpp"
#include <NeoPixelBus.h>

class StripController {
    NeoPixelBus<NeoRgbFeature, NeoWs2811Method> m_strip;
public:
      StripController(const uint16_t holdAmount, const uint8_t outputPin);
      void runInitAnimation(void);
      void showBoard(const std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> &holds, const bool showMoveBeta = false);
      void showBoard(void);
}; 


extern StripController stripController;