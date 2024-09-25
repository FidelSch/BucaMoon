#pragma once

#include "defines.hpp"
// #include <Arduino.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>
#include <BLEDevice.h>

extern String problem;
extern uint8_t holds[HOLD_AMOUNT];
extern uint8_t ledmapping[HOLD_AMOUNT];


void clearBoard();
void printBoardState(uint8_t holds[HOLD_AMOUNT]);
void parseProblemString(String problem, uint8_t newHolds[HOLD_AMOUNT]);
void showBoard(uint8_t holds[HOLD_AMOUNT]);

class MoonCallback : public BLECharacteristicCallbacks{
public:
      void onWrite(BLECharacteristic *pCharacteristic);
};