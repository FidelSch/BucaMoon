#include "defines.hpp"
#include "Moonboard.hpp"
#include "Strip.hpp"
#include "MoonboardServer.hpp"

#include <Arduino.h>


void setup()
{
      showBoard();
      runInitAnimation();
      initServer();
}

void loop()
{
      if (clientCount() < MOONBOARD_MAX_CONNECTIONS)
      {
            ESP_LOGD("Server", "%d/%d clients connected. Advertising...");
            startAdvertising();
      }

      vTaskDelay(2000 / portTICK_PERIOD_MS);
}
