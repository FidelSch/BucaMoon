#include "Strip.hpp"
#include "Hold.hpp"
#include <Adafruit_NeoPixel.h>

static Adafruit_NeoPixel strip(HOLD_AMOUNT, OUTPUT_PIN, NEO_RGB | NEO_KHZ800);

void initAnimation(void *_null){
      strip.begin();
      // Red, green, blue, off
      // uint32_t colors[4] = {0xFF0000, 0x00FF00, 0x0000FF, 0x000000};
      uint32_t color = 0x00FF0000;

      while (color)
      {
            for (size_t i = 0; i < HOLD_AMOUNT; i++)
            {
                  strip.setPixelColor(i, color);
            }
            vTaskDelay(1000);
            strip.show();
            color >>= 8;
      }

      vTaskDelete(NULL); // End this task

      // This function should never return
      for(;;) vTaskDelay(5000);
}

void showBoard(const std::array<uint8_t, HOLD_AMOUNT> holds)
{
      strip.begin();

      for (size_t i = 0; i < HOLD_AMOUNT; i++)
      {
            switch (holds[i])
            {
            case Hold::START_HOLD:
                  strip.setPixelColor(i, 0x00FF00);
                  break;
            case Hold::RIGHT_HOLD:
            case Hold::LEFT_HOLD:
            case Hold::MATCH_HOLD:
            case Hold::PROGRESS_HOLD:
            case Hold::FOOT_HOLD:
            case Hold::ADDITIONAL_LED:
                  strip.setPixelColor(i, 0x0000FF);
                  break;
            case Hold::END_HOLD:
                  strip.setPixelColor(i, 0xFF0000);
                  break;
            case Hold::NO_HOLD:
            default:
                  strip.setPixelColor(i, 0x000000);
                  break;
            }
      }

      strip.show();
}