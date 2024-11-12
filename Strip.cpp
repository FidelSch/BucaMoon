#include "Strip.hpp"
#include "Hold.hpp"
#include <Adafruit_NeoPixel.h>

constexpr uint32_t RGB_RED = 0xFF0000;
constexpr uint32_t RGB_GREEN = 0x00FF00;
constexpr uint32_t RGB_BLUE = 0x0000FF;
constexpr uint32_t RGB_BLACK = 0x000000;

static Adafruit_NeoPixel strip(HOLD_AMOUNT, OUTPUT_PIN, NEO_RGB | NEO_KHZ800);

/// @brief Runs init animation on core 0
void runInitAnimation(void)
{
      TaskHandle_t AnimationHandle;

      xTaskCreatePinnedToCore(
          [](void *)
          {
                strip.begin(); 
                uint32_t color = RGB_RED;
                while (color)
                {
                      for (size_t i = 0; i < HOLD_AMOUNT; i++)
                      {
                            strip.setPixelColor(i, color);
                      }
                      strip.show();
                      vTaskDelay(1000);
                      color >>= 8;
                }
                strip.clear();
                strip.show();
                vTaskDelete(NULL); // End this task
          },
          "initAnimation", 4096, NULL, 1, &AnimationHandle, 0);
}

/// @brief Translates hold buffer into board
/// @param holds Hold buffer
void showBoard(const std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> holds)
{
      strip.begin();

      for (size_t i = 0; i < HOLD_AMOUNT; i++)
      {
            switch (holds[i])
            {
            case Hold::START_HOLD:
                  strip.setPixelColor(i, RGB_GREEN);
                  break;
            case Hold::RIGHT_HOLD:
            case Hold::LEFT_HOLD:
            case Hold::MATCH_HOLD:
            case Hold::PROGRESS_HOLD:
            case Hold::FOOT_HOLD:
            case Hold::ADDITIONAL_LED:
                  strip.setPixelColor(i, RGB_BLUE);
                  break;
            case Hold::END_HOLD:
                  strip.setPixelColor(i, RGB_RED);
                  break;
            case Hold::NO_HOLD:
            default:
                  strip.setPixelColor(i, RGB_BLACK);
                  break;
            }
      }

      strip.show();
}