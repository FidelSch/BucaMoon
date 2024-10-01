#pragma once

#include "defines.hpp"
#include <BLEDevice.h>


/// @brief Show init animation
void initAnimation(void *_null = nullptr);

/// @brief Print detailed board state to serial out based on hold representation
/// @param holds Hold representation
void printBoardState(const std::array<uint8_t, HOLD_AMOUNT> &holds);

/// @brief Parses input string into output hold representation array
/// @param problemString Complete problem string
/// @param outHolds Hold representation output
void parseProblemString(const String &problemString, std::array<uint8_t, HOLD_AMOUNT> &outHolds);

/// @brief "Prints" hold array into LED strip
/// @param holds Hold representation to be printed
void showBoard(const std::array<uint8_t, HOLD_AMOUNT> holds);

class MoonCallback : public BLECharacteristicCallbacks{
public:
      void onWrite(BLECharacteristic *pCharacteristic);
};