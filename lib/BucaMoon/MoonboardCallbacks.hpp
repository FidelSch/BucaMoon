#pragma once
#include "Problem.hpp"

#include <BLECharacteristic.h>
#include <BLEServer.h>

class MoonboardCharacteristicCallback : public BLECharacteristicCallbacks{
      std::string m_problemString;
      Problem m_problem;
public:
      MoonboardCharacteristicCallback();
      void onWrite(BLECharacteristic *pCharacteristic) override;
};

class MoonboardServerCallback : public BLEServerCallbacks{
public:
	void onConnect(BLEServer* pServer) override;
	void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) override;
};