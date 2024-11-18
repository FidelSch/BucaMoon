#include "defines.hpp"
#include "Moonboard.hpp"
#include "Strip.hpp"
#include "MoonboardServer.hpp"

#include <Arduino.h>


void setup()
{
#ifdef _DEBUG
	Serial.begin(115200);
	Serial.println("Laburando...");
#endif

      runInitAnimation();
      initServer();

#ifdef _DEBUG
      Serial.println("Listo!");
#endif
}

void loop()
{
      if (clientCount() < MOONBOARD_MAX_CONNECTIONS)
            startAdvertising();

#ifdef _DEBUG
      Serial.println("Conectados: " + String(clientCount()));
#endif
      vTaskDelay(2000 / portTICK_PERIOD_MS);
}
