#pragma once

#include "defines.hpp"
#include <BLECharacteristic.h>
#include <Arduino.h>


void printBoardState(const std::array<uint8_t, HOLD_AMOUNT> &holds);
void parseProblemString(const String &problemString, std::array<uint8_t, HOLD_AMOUNT> &outHolds);

class MoonCallback : public BLECharacteristicCallbacks{
public:
      void onWrite(BLECharacteristic *pCharacteristic) override;
};