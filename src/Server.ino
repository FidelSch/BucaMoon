#include "defines.hpp"
#include "Moonboard.hpp"
#include "Strip.hpp"
#include "MoonboardServer.hpp"

#include <Arduino.h>


void setup()
{
      showBoard();
      runInitAnimation();
      MoonboardServer::init();
}

void loop()
{
      if (MoonboardServer::clientCount() < MOONBOARD_MAX_CONNECTIONS)
      {
            ESP_LOGD("Server", "%d/%d clients connected. Advertising...", MoonboardServer::clientCount(), MOONBOARD_MAX_CONNECTIONS);
            MoonboardServer::startAdvertising();
      }

      vTaskDelay(5000 / portTICK_PERIOD_MS);
}
