#include "defines.hpp"
#include "Moonboard.hpp"
#include "Strip.hpp"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

static BLEServer *Server;

void setup()
{
#ifdef _DEBUG
	Serial.begin(115200);
	Serial.println("Laburando...");
#endif

      runInitAnimation();

      BLEDevice::init(MOONBOARD_DEVICE_NAME);
	Server = BLEDevice::createServer();
	BLEService *Service = Server->createService(SERVICE_UUID);
	BLECharacteristic *RXCharacteristic = Service->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);

	RXCharacteristic->setCallbacks(new MoonboardCharacteristicCallback());

	Service->start();

	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(true);
	pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
	pAdvertising->setMinPreferred(0x12);
#ifdef _DEBUG
      Serial.println("Listo!");
#endif
}

void loop()
{
      if (Server->getConnectedCount() < MOONBOARD_MAX_CONNECTIONS)
            BLEDevice::startAdvertising();

#ifdef _DEBUG
      Serial.println("Conectados: " + String(Server->getConnectedCount()));
#endif
      vTaskDelay(2000 / portTICK_PERIOD_MS);
}
