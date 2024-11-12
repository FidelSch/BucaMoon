#pragma once

#include "defines.hpp"
#include <BLECharacteristic.h>
#include <Arduino.h>

// Status codes
#define PARSE_OK 0
#define PARSE_ERROR_OUT_OF_BOUNDS 1

#ifdef _DEBUG
void printBoardState(const std::array<uint8_t, HOLD_AMOUNT> &holds);
#endif

void parseProblemString(const String &problemString, std::array<uint8_t, HOLD_AMOUNT> *outHolds);

class MoonboardCharacteristicCallback : public BLECharacteristicCallbacks{
public:
      void onWrite(BLECharacteristic *pCharacteristic) override;
};