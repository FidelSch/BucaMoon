#include "MoonboardCallbacks.hpp"

#include <Arduino.h>

MoonboardCharacteristicCallback::MoonboardCharacteristicCallback()
{
      m_problemString.reserve(75); // Theoretical max for problem string
}

void MoonboardCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
      const std::string value = pCharacteristic->getValue();

      ESP_LOGI("BLE Write", "Receiving data...");
      ESP_LOGD("BLE Write", "Received: %s", value.c_str());

      if (value.length() == 0)
            return;

      if ('~' == value[0] || 'l' == value[0]) // Problem start
            m_problemString = "";

      for (size_t i = 0; i < value.length(); i++)
      {
            if (i == value.length() - 1 && (value[i] == '#' || value[i] == '*')) // Problem end
            {
                  m_problem = Problem(m_problemString);
                  m_problem.process();

#if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
                  m_problem.printBoardState();
#endif
                  return;
            }

            m_problemString += value[i];
      }
}