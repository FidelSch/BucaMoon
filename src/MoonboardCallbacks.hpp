#pragma once
#include "Problem.hpp"

#include <BLECharacteristic.h>

class MoonboardCharacteristicCallback : public BLECharacteristicCallbacks{
      std::string m_problemString;
      Problem m_problem;
public:
      MoonboardCharacteristicCallback();
      void onWrite(BLECharacteristic *pCharacteristic) override;
};