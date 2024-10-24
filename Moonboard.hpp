#pragma once

#include "defines.hpp"
#include <BLECharacteristic.h>
#include <Arduino.h>

#ifdef _DEBUG
void printBoardState(const std::array<uint8_t, HOLD_AMOUNT> &holds);
#endif

void parseProblemString(const String &problemString, std::array<uint8_t, HOLD_AMOUNT> &outHolds);

class MoonboardCharacteristicCallback : public BLECharacteristicCallbacks{
public:
      void onWrite(BLECharacteristic *pCharacteristic) override;
};