#include "Strip.hpp"
#include "Hold.hpp"
#include <Adafruit_NeoPixel.h>

CRGB LedBuffer[HOLD_AMOUNT];

/// @brief Runs init animation on core 0
void runInitAnimation(void)
{
      TaskHandle_t AnimationHandle;

      xTaskCreatePinnedToCore(
          [](void *)
          {
                // Red, green, blue, off
                std::array<CRGB, 4> colors = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Black};

                for (auto colo : colors)
                {
                      for (size_t i = 0; i < HOLD_AMOUNT; i++)
                      {
                            LedBuffer[i] = colo;
                      }
                      vTaskDelay(1000);
                      FastLED.show();
                }

                vTaskDelete(NULL); // End this task
          },
          "initAnimation", 4096, NULL, 1, &AnimationHandle, 0);
}

/// @brief "Prints" hold array into LED strip
/// @param holds Hold representation to be printed
void showBoard(const std::array<uint8_t, HOLD_AMOUNT> holds)
{

      for (size_t i = 0; i < HOLD_AMOUNT; i++)
      {
            switch (holds[i])
            {
            case Hold::START_HOLD:
                  LedBuffer[i] = CRGB::Green;
                  break;
            case Hold::RIGHT_HOLD:
            case Hold::LEFT_HOLD:
            case Hold::MATCH_HOLD:
            case Hold::PROGRESS_HOLD:
            case Hold::FOOT_HOLD:
            case Hold::ADDITIONAL_LED:
                  LedBuffer[i] = CRGB::Blue;
                  break;
            case Hold::END_HOLD:
                  LedBuffer[i] = CRGB::Red;
                  break;
            case Hold::NO_HOLD:
            default:
                  LedBuffer[i] = CRGB::Black;
                  break;
            }
      }

      FastLED.show();
}