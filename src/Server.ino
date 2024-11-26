#include "defines.hpp"
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
      // Nothing to do
      vTaskDelay(5000 / portTICK_PERIOD_MS);
}
