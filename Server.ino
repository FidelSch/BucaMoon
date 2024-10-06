#include <NeoPixelBus.h>


#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "defines.hpp"
#include "BucaMoon.hpp"

TaskHandle_t Animation;

BLEServer *pServer;

void setup()
{
#ifdef DEBUG
	Serial.begin(115200);
	Serial.println("Laburando...");
#endif

      gpio_set_direction((gpio_num_t)PIN, GPIO_MODE_OUTPUT);

      xTaskCreatePinnedToCore(
          initAnimation,
          "initAnimation",
          4096,
          NULL,
          1,
          &Animation,
          0);

      BLEDevice::init("Fran se la come");
	pServer = BLEDevice::createServer();
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

#ifdef DEBUG
      Serial.println("Listo!");
#endif
}

void loop()
{
      if (pServer->getConnectedCount() < 4){
            BLEDevice::startAdvertising();
      }

      #ifdef DEBUG
	Serial.println("Conectados: " + String(pServer->getConnectedCount()));
	vTaskDelay(2000 / portTICK_PERIOD_MS);
      #endif
}
