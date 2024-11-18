#include "defines.hpp"
#include "Moonboard.hpp"
#include "Strip.hpp"
#include "Hold.hpp"
#include "MoonboardServer.hpp"

#include <Arduino.h>


#ifdef _DEBUG

/// @brief Print detailed board state to serial out based on hold representation
/// @param holds Hold representation
void printBoardState(const std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> &holds)
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
            if (holds[ledPrintMapping[j]] != Hold::NO_HOLD)
                  Serial.print("[" + String((char)(holds[ledPrintMapping[j]])) + "]");
            else
                  Serial.print("[ ]");
      }
      Serial.println();
}
#endif

/// @brief Parses input string into output hold representation array
/// @param problemString Complete problem string
/// @param outHolds Hold representation output
void parseProblemString(const String &problemString, std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> *outHolds)
{
      String holdDescription = "";
      size_t holdIndex;
      Hold::HOLDTYPE_t holdType;

      // Helper function
      auto parseHold = [](const String &description, size_t *outIndex, Hold::HOLDTYPE_t *outHold)
      {
            *outIndex = description.substring(1).toInt();
            *outHold = (Hold::HOLDTYPE_t)description[0];

            if (*outIndex >= HOLD_AMOUNT)
            {
                  Serial.println("ERROR: Hold Index out of range"); // Buffer overflow protection
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
                  holdDescription.concat(problemString[i]);
            }
      }
}

/// @brief  Set an additional led for each marked hold, if they have one
/// @param holds Hold representation array
void setAdditionalLeds(std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> *holds)
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

      for (size_t i = 0; i <(*holds).size(); i++)
      {
            uint8_t mapping = additionalLedMapping(i);
            if ((*holds)[mapping] != Hold::NO_HOLD || mapping == NO_MAPPING)
                  continue;

            if ((*holds)[i] != Hold::ADDITIONAL_LED && (*holds)[i] != Hold::NO_HOLD)
                  (*holds)[mapping] = Hold::ADDITIONAL_LED;
      }
}

void processProblem(const String &problemString, std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> &holds, const char configuration)
{
      parseProblemString(problemString.c_str(), &holds);

      switch (configuration)
      {
      case 'Z': // "Disconnect all clients"
            disconnectAllClients();
            break;
      case 'D':
            setAdditionalLeds(&holds);
            showBoard(holds, false);
            break;
      case 'B': // "Show move beta"
            showBoard(holds, true);
            break;
      default:
            showBoard(holds, false);
      }
}

void MoonboardCharacteristicCallback::onWrite(BLECharacteristic *pCharacteristic)
{
      static std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> holds;
      static std::string problemString = "";
      static char configuration = 0;

      std::string value = pCharacteristic->getValue();
      size_t i = 0;

#ifdef _DEBUG
      Serial.println("Recibido: " + String(value.c_str()));
#endif

      if (value[0] == '~') // Configuration
      {
#ifdef _DEBUG
            Serial.println("Configuration:" + String(value[1]));
#endif
            configuration = value[1];

            // Go to start of problem string
            i = 3;
      }

      if (i < value.length() && value.length() > 1 && value[i] == 'l' && value[i + 1] == '#') // New problem
      {
            problemString = "";
            holds.fill(Hold::NO_HOLD);
            showBoard();
            i+=2;
      }
      while (i < value.length())
      {
            if (value[i] == '#') // Problem end
            {
                  processProblem(problemString.c_str(), holds, configuration);
                  // Reset configuration
                  configuration = 0;
#ifdef _DEBUG
                  printBoardState(holds);
                  Serial.println(problemString.c_str());
                  Serial.print("holds: ");
                  for (char c: holds) Serial.print(String((c)?c:'+'));
                  Serial.println();
#endif
                  return;
            }

            problemString += value[i];
            i++;
      }
}