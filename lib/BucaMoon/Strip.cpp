#include "Strip.hpp"
#include "Hold.hpp"

constexpr uint32_t RGB_RED       = 0xFF0000;
constexpr uint32_t RGB_GREEN     = 0x00FF00;
constexpr uint32_t RGB_BLUE      = 0x0000FF;
constexpr uint32_t RGB_BLACK     = 0x000000;
constexpr uint32_t RGB_LIGHTBLUE = 0x48C2FF;
constexpr uint32_t RGB_PURPLE    = 0x800080;
constexpr uint32_t RGB_SALMON    = 0xFF5A2C;

/// @brief Constructor
/// @param holdAmount Amount of holds
/// @param outputPin Output pin
StripController::StripController(const uint16_t holdAmount, const uint8_t outputPin)
    : m_strip(holdAmount, outputPin)
{
      m_strip.Begin();
}

/// @brief Runs init animation on core 0
/// @warning May strain power supply, use with caution
/// Consider avoiding if you experience crashes
void StripController::runInitAnimation(void)
{
      ESP_LOGI("initAnimation", "Running animation");
      TaskHandle_t AnimationHandle;

      xTaskCreatePinnedToCore(
          [](void * p_strip)
          {
                auto strip = static_cast<NeoPixelBus<NeoRgbFeature, NeoWs2811Method> *>(p_strip);
                uint32_t color = 0x880000;
                while (color)
                {
                      ESP_LOGD("Init Animation", "Showing %d", color);
                      strip->ClearTo(HtmlColor(color));
                      strip->Show();
                      vTaskDelay(1000);
                      color >>= 8;
                }
                strip->ClearTo(HtmlColor(RGB_BLACK));
                strip->Show();
                vTaskDelete(NULL); // End this task
          },
          "initAnimation", 
          4096, 
          &(this->m_strip), 
          1, 
          &AnimationHandle, 
          0);
}

/// @brief Translates hold buffer into board
/// @param holds Hold buffer
/// @param showMoveBeta Show beta colors
void StripController::showBoard(const std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> holds, const bool showMoveBeta)
{
      ESP_LOGI("showBoard", "Updating LEDs");
      static const uint32_t START_COLOR      = RGB_GREEN;
      static const uint32_t RIGHT_COLOR      = RGB_BLUE;
      static const uint32_t PROGRESS_COLOR   = RGB_BLUE;
      static const uint32_t ADDITIONAL_COLOR = RGB_BLUE;
      static const uint32_t END_COLOR        = RGB_RED;
      static const uint32_t NO_COLOR         = RGB_BLACK;

      // Optional beta colors
      uint32_t LEFT_COLOR       = RGB_BLUE;
      uint32_t FOOT_COLOR       = RGB_BLUE;
      uint32_t MATCH_COLOR      = RGB_BLUE;

      if (showMoveBeta)
      {
            LEFT_COLOR = RGB_PURPLE;
            FOOT_COLOR = RGB_LIGHTBLUE;
            MATCH_COLOR = RGB_SALMON;
      }

      for (size_t i = 0; i < HOLD_AMOUNT; i++)
      {
            switch (holds[i])
            {
            case Hold::START_HOLD:
                  m_strip.SetPixelColor(i, HtmlColor(START_COLOR));
                  break;
            case Hold::RIGHT_HOLD:
                  m_strip.SetPixelColor(i, HtmlColor(RIGHT_COLOR));
                  break;
            case Hold::LEFT_HOLD:
                  m_strip.SetPixelColor(i, HtmlColor(LEFT_COLOR));
                  break;
            case Hold::MATCH_HOLD:
                  m_strip.SetPixelColor(i, HtmlColor(MATCH_COLOR));
                  break;
            case Hold::PROGRESS_HOLD:
                  m_strip.SetPixelColor(i, HtmlColor(PROGRESS_COLOR));
                  break;
            case Hold::FOOT_HOLD:
                  m_strip.SetPixelColor(i, HtmlColor(FOOT_COLOR));
                  break;
            case Hold::ADDITIONAL_LED:
                  m_strip.SetPixelColor(i, HtmlColor(ADDITIONAL_COLOR));
                  break;
            case Hold::END_HOLD:
                  m_strip.SetPixelColor(i, HtmlColor(END_COLOR));
                  break;
            case Hold::NO_HOLD:
            default:
                  m_strip.SetPixelColor(i, HtmlColor(NO_COLOR));
                  break;
            }
      }

      m_strip.Show();
}

/// @brief Clears the board
void StripController::showBoard(void)
{
      ESP_LOGI("showBoard", "Clearing LEDs");
      m_strip.ClearTo(HtmlColor(RGB_BLACK));
      ESP_LOGV("showBoard", "Showing...");
      m_strip.Show();
      ESP_LOGV("showBoard", "Cleared");
}