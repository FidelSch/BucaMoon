#include <NeoPixelBus.h>


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "defines.hpp"
#include "BucaMoon.hpp"


void setup()
{
	Serial.begin(115200);
	Serial.println("Laburando...");

	BLEDevice::init("CABA es mejor que AEBA");
	BLEServer *pServer = BLEDevice::createServer();
	BLEService *pService = pServer->createService(SERVICE_UUID);
	BLECharacteristic *pCharacteristicRX = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
	BLECharacteristic *pCharacteristicTX = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_READ);

	pCharacteristicRX->setCallbacks(new MoonCallback());

	pCharacteristicTX->setValue("Si podes leer esto a Rena le gustan las 2009");
	pService->start();
	// BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(true);
	pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
	pAdvertising->setMinPreferred(0x12);
	BLEDevice::startAdvertising();

      initAnimation();
      // Show empty board
      showBoard(NULL);
      Serial.println("Listo!");
}

void loop()
{
      #ifdef DEBUG
	Serial.println("*");
	delay(2000);
      #endif
}
