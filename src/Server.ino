#include "defines.hpp"
#include "Strip.hpp"
#include "MoonboardServer.hpp"

#include <Arduino.h>

StripController stripController(HOLD_AMOUNT, OUTPUT_PIN);
void setup()
{
      stripController.showBoard();
      stripController.runInitAnimation();
      MoonboardServer::init();
}

void loop()
{
      // Nothing to do
      vTaskDelay(5000 / portTICK_PERIOD_MS);
}
