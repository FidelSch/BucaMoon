#pragma once

#include "defines.hpp"
// #include <Arduino.h>
// #include <BLEUtils.h>
// #include <BLEServer.h>
#include <BLEDevice.h>

// extern String problemString;
// extern uint8_t holds[HOLD_AMOUNT];
// extern uint8_t ledmapping[HOLD_AMOUNT];
// extern bool use_additional_led;


void clearProblem(uint8_t h[HOLD_AMOUNT]);
void printBoardState(uint8_t holds[HOLD_AMOUNT]);
void parseProblemString(String problemString, uint8_t newHolds[HOLD_AMOUNT]);
void showBoard(uint8_t holds[HOLD_AMOUNT] = nullptr);

class MoonCallback : public BLECharacteristicCallbacks{
public:
      void onWrite(BLECharacteristic *pCharacteristic);
};