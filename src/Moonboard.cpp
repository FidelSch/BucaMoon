#include "Moonboard.hpp"
#include "Problem.hpp"

#include <Arduino.h>

void MoonboardCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
      static Problem problem;
      static std::string problemString = "";

      std::string value = pCharacteristic->getValue();
      size_t i = 0;

      ESP_LOGI("BLE Write", "Receiving data...");
      ESP_LOGD("BLE Write", "Received: %s", value.c_str());

      while (i < value.length())
      {
            if (i == value.length() - 1 && (value[i] == '#' || value[i] == '*')) // Problem end
            {
                  problem = Problem(problemString);
                  problem.process();
                  problemString = "";

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
                  problem.printBoardState();
#endif
                  return;
            }

            problemString += value[i];
            i++;
      }
}