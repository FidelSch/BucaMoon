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


void initAnimation();
void printBoardState(std::array<uint8_t, HOLD_AMOUNT> &holds);
void parseProblemString(String problemString, std::array<uint8_t, HOLD_AMOUNT> &newHolds);
void showBoard(const std::array<uint8_t, HOLD_AMOUNT> holds);

class MoonCallback : public BLECharacteristicCallbacks{
public:
      void onWrite(BLECharacteristic *pCharacteristic);
};