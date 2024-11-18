#include "Strip.hpp"
#include "Hold.hpp"
#include <Adafruit_NeoPixel.h>

constexpr uint32_t RGB_RED       = 0xFF0000;
constexpr uint32_t RGB_GREEN     = 0x00FF00;
constexpr uint32_t RGB_BLUE      = 0x0000FF;
constexpr uint32_t RGB_BLACK     = 0x000000;
constexpr uint32_t RGB_LIGHTBLUE = 0x48C2FF;
constexpr uint32_t RGB_VIOLET    = 0xAA48FF;
constexpr uint32_t RGB_PINK      = 0xFF7BAB;

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
/// @param showMoveBeta 
void showBoard(const std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> holds, const bool showMoveBeta)
{
      const uint32_t START_COLOR      = RGB_GREEN;
      const uint32_t RIGHT_COLOR      = RGB_BLUE;
      const uint32_t PROGRESS_COLOR   = RGB_BLUE;
      const uint32_t ADDITIONAL_COLOR = RGB_BLUE;
      const uint32_t END_COLOR        = RGB_RED;
      const uint32_t NO_COLOR         = RGB_BLACK;

      // Optional beta colors
      uint32_t LEFT_COLOR       = RGB_BLUE;
      uint32_t FOOT_COLOR       = RGB_BLUE;
      uint32_t MATCH_COLOR      = RGB_BLUE;

      if (showMoveBeta)
      {
            LEFT_COLOR = RGB_VIOLET;
            FOOT_COLOR = RGB_LIGHTBLUE;
            MATCH_COLOR = RGB_PINK;
      }

      strip.begin();

      for (size_t i = 0; i < HOLD_AMOUNT; i++)
      {
            switch (holds[i])
            {
            case Hold::START_HOLD:
                  strip.setPixelColor(i, START_COLOR);
                  break;
            case Hold::RIGHT_HOLD:
                  strip.setPixelColor(i, RIGHT_COLOR);
                  break;
            case Hold::LEFT_HOLD:
                  strip.setPixelColor(i, LEFT_COLOR);
                  break;
            case Hold::MATCH_HOLD:
                  strip.setPixelColor(i, MATCH_COLOR);
                  break;
            case Hold::PROGRESS_HOLD:
                  strip.setPixelColor(i, PROGRESS_COLOR);
                  break;
            case Hold::FOOT_HOLD:
                  strip.setPixelColor(i, FOOT_COLOR);
                  break;
            case Hold::ADDITIONAL_LED:
                  strip.setPixelColor(i, ADDITIONAL_COLOR);
                  break;
            case Hold::END_HOLD:
                  strip.setPixelColor(i, END_COLOR);
                  break;
            case Hold::NO_HOLD:
            default:
                  strip.setPixelColor(i, NO_COLOR);
                  break;
            }
      }

      strip.show();
}

void showBoard(void)
{
      strip.clear();
      strip.show();
}