#include "MoonboardServer.hpp"
#include "defines.hpp"
#include "MoonboardCallbacks.hpp"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <Arduino.h>

static BLEServer *Server;

void MoonboardServer::init(void)
{
      ESP_LOGI("Server","Starting BLE Server");

      BLEDevice::init(MOONBOARD_DEVICE_NAME);
	Server = BLEDevice::createServer();
	BLEService *Service = Server->createService(SERVICE_UUID);
	BLECharacteristic *RXCharacteristic = Service->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

	RXCharacteristic->setCallbacks(new MoonboardCharacteristicCallback());
      Server->setCallbacks(new MoonboardServerCallback());

	Service->start();

	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(true);
	pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
	pAdvertising->setMinPreferred(0x12);

      Server->startAdvertising();
      ESP_LOGI("Server","BLE Server started");
}

int MoonboardServer::clientCount(void)
{
      return Server->getConnectedCount();
}

void MoonboardServer::startAdvertising(void)
{
      BLEDevice::startAdvertising();
}

void MoonboardServer::disconnectAllClients(void)
{
      ESP_LOGI("disconnectAllClients","Client disconnection called. Proceeding");
      auto clients = Server->getPeerDevices(true);

      for (auto i: clients)
      {
            Server->disconnect(i.first);
      }
}