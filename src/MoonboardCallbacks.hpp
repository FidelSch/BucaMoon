#pragma once

#include <BLECharacteristic.h>

class MoonboardCharacteristicCallback : public BLECharacteristicCallbacks{
public:
      void onWrite(BLECharacteristic *pCharacteristic) override;
};