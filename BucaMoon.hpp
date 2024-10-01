#pragma once

#include "defines.hpp"
#include <BLEDevice.h>


void initAnimation(void *_null);
void printBoardState(std::array<uint8_t, HOLD_AMOUNT> &holds);
void parseProblemString(String problemString, std::array<uint8_t, HOLD_AMOUNT> &newHolds);
void showBoard(const std::array<uint8_t, HOLD_AMOUNT> holds);

class MoonCallback : public BLECharacteristicCallbacks{
public:
      void onWrite(BLECharacteristic *pCharacteristic);
};