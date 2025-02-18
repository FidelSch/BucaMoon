#include "Problem.hpp"
#include "defines.hpp"
#include "Hold.hpp"
#include "MoonboardServer.hpp"
#include "Strip.hpp"

#include <Arduino.h>

Problem::Problem(void)
{
      m_holds = std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT>();
      m_holds.fill(Hold::NO_HOLD);
      m_configuration = 0;
      m_problemString = "";
}

Problem::Problem(const std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> &holds, const char configuration)
{
      m_holds = holds;
      m_configuration = configuration;
      m_problemString = "";
}

Problem::Problem(std::string problemString)
{
      m_holds = std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT>();
      m_holds.fill(Hold::NO_HOLD);
      m_problemString = problemString;
      size_t problem_start_pos = 2; // After "l#" (problem start)

      if (problemString.length() >= 2 && problemString[0] == '~') // Has configuration
      {
            m_configuration = problemString[1];

            // After "~C*l#", where C is the configuration char
            problem_start_pos = 5;
      }
      else 
            m_configuration = 0;

      // String may not actually contain a problem 
      if (problemString.length() <= problem_start_pos)
            return;

      parseProblemString(problemString.substr(problem_start_pos), &m_holds);
      ESP_LOGV("Problem constructor", "%s", m_problemString.c_str());
}

void Problem::process(void)
{
      ESP_LOGI("processProblem", "Processing %s", m_problemString.c_str());

      switch (m_configuration)
      {
      case 'Z': // "Disconnect all clients"
            MoonboardServer::disconnectAllClients();
            break;
      case 'D': // "Use additional LED"
            setAdditionalLeds();
            stripController.showBoard(m_holds, false);
            break;
      case 'B': // "Show move beta"
            stripController.showBoard(m_holds, true);
            break;
      default:
            stripController.showBoard(m_holds, false);
      }
      ESP_LOGI("processProblem", "Done");
}

/// @brief Parses input string into output hold representation array
///   Static method
/// @param problemString Complete problem string
/// @param outHolds Hold representation output
void Problem::parseProblemString(const std::string &problemString, std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> *outHolds)
{
      ESP_LOGD("parseProblemString", "Parsing %s", problemString.c_str());

      if (problemString.empty())
      {
            outHolds->fill(Hold::NO_HOLD);
            return;
      } 
      std::string holdDescription = "";
      holdDescription.reserve(4); // Up to 3 digits + description character
      size_t holdIndex;
      Hold::HOLDTYPE_t holdType;

      constexpr int8_t PARSE_ERROR_OUT_OF_BOUNDS = -1;
      constexpr int8_t PARSE_ERROR_INVALID_INDEX = -2;
      constexpr int8_t PARSE_OK = 0;

      // Helper function
      auto parseHold = [](const std::string &description, size_t *outIndex, Hold::HOLDTYPE_t *outHold)
      {
            try {
                  *outIndex = stoi(description.substr(1));
            } catch (const std::invalid_argument &e) {
                  ESP_LOGE("parseProblemString", "ERROR: Hold Index is not a number! Got %s", description.c_str());
                  return PARSE_ERROR_INVALID_INDEX;
            }

            *outHold = static_cast<Hold::HOLDTYPE_t>(description[0]);

            if (*outIndex >= HOLD_AMOUNT || *outIndex < 0)
            {
                  ESP_LOGE("parseProblemString", "ERROR: Hold Index out of range"); // Buffer overflow protection
                  return PARSE_ERROR_OUT_OF_BOUNDS;
            }
            return PARSE_OK;
      };

      // Parse each hold
      for (size_t i = 0; i <= problemString.length(); i++)
      {
            if (i == problemString.length() || problemString[i] == ',')
            {
                  if (parseHold(holdDescription, &holdIndex, &holdType) != PARSE_OK)
                        return;

                  (*outHolds)[holdIndex] = holdType;
                  holdDescription.clear();
            }
            else
            {
                  holdDescription.append(1, problemString[i]);
            }
      }
      ESP_LOGD("parseProblemString", "Done");
}

/// @brief  Set an additional led for each marked hold, if they have one
void Problem::setAdditionalLeds()
{
      constexpr int8_t NO_MAPPING = INT8_MAX;
      static const std::array<int8_t, HOLD_AMOUNT> _additionalledmapping = {
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, NO_MAPPING,                  // 1
          NO_MAPPING, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 2
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, NO_MAPPING,                  // 3
          NO_MAPPING, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 4
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, NO_MAPPING,                  // 5
          NO_MAPPING, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 6
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, NO_MAPPING,                  // 7
          NO_MAPPING, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 8
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, NO_MAPPING,                  // 9
          NO_MAPPING, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 10
          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, NO_MAPPING                   // 11
      };

      auto additionalLedMapping = [](size_t i)
      {
            return (i < _additionalledmapping.size()) ? i + _additionalledmapping[i] : NO_MAPPING;
      };

      for (size_t i = 0; i < m_holds.size(); i++)
      {
            uint8_t mapping = additionalLedMapping(i);
            if (mapping >= HOLD_AMOUNT || m_holds[mapping] != Hold::NO_HOLD)
                  continue;

            if (m_holds[i] != Hold::ADDITIONAL_LED && m_holds[i] != Hold::NO_HOLD)
                  m_holds[mapping] = Hold::ADDITIONAL_LED;
      }
}

std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> Problem::getHolds()
{
      return m_holds;
}

char Problem::getConfiguration()
{
      return m_configuration;
}

/// @brief Print detailed board state to serial out based on hold representation
/// @param holds Hold representation
void Problem::printBoardState()
{
      static const uint8_t ledPrintMapping[HOLD_AMOUNT] = {17, 18, 53, 54, 89, 90, 125, 126, 161, 162, 197,
                                                           16, 19, 52, 55, 88, 91, 124, 127, 160, 163, 196,
                                                           15, 20, 51, 56, 87, 92, 123, 128, 159, 164, 195,
                                                           14, 21, 50, 57, 86, 93, 122, 129, 158, 165, 194,
                                                           13, 22, 49, 58, 85, 94, 121, 130, 157, 166, 193,
                                                           12, 23, 48, 59, 84, 95, 120, 131, 156, 167, 192,
                                                           11, 24, 47, 60, 83, 96, 119, 132, 155, 168, 191,
                                                           10, 25, 46, 61, 82, 97, 118, 133, 154, 169, 190,
                                                           9, 26, 45, 62, 81, 98, 117, 134, 153, 170, 189,
                                                           8, 27, 44, 63, 80, 99, 116, 135, 152, 171, 188,
                                                           7, 28, 43, 64, 79, 100, 115, 136, 151, 172, 187,
                                                           6, 29, 42, 65, 78, 101, 114, 137, 150, 173, 186,
                                                           5, 30, 41, 66, 77, 102, 113, 138, 149, 174, 185,
                                                           4, 31, 40, 67, 76, 103, 112, 139, 148, 175, 184,
                                                           3, 32, 39, 68, 75, 104, 111, 140, 147, 176, 183,
                                                           2, 33, 38, 69, 74, 105, 110, 141, 146, 177, 182,
                                                           1, 34, 37, 70, 73, 106, 109, 142, 145, 178, 181,
                                                           0, 35, 36, 71, 72, 107, 108, 143, 144, 179, 180};
      // 18 rows, 11 columns
      Serial.print("   [A][B][C][D][E][F][G][H][I][J][K]");
      uint8_t row = 18;
      for (int j = 0; j < HOLD_AMOUNT; j++)
      {
            if (j % 11 == 0)
            { // new row
                  // Align with header
                  if (row < 10)
                        Serial.print("\n0" + String(row--) + "-");
                  else
                        Serial.print("\n" + String(row--) + "-");
            }

            // Print holds
            if (m_holds[ledPrintMapping[j]] != Hold::NO_HOLD)
                  Serial.print("[" + String((char)(m_holds[ledPrintMapping[j]])) + "]");
            else
                  Serial.print("[ ]");
      }
      Serial.println();
}