#include "defines.hpp"
#include "BucaMoon.hpp"
#include <NeoPixelBus.h>

String problem = "";
uint8_t holds[HOLD_AMOUNT] = "";

#ifdef DEBUG
uint8_t ledPrintMapping[HOLD_AMOUNT] = {17, 18, 53, 54, 89, 90, 125, 126, 161, 162, 197,
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
#endif

void clearBoard()
{
	for (int i = 0; i < HOLD_AMOUNT; i++)
	{
		holds[i] = 0;
	}
}

#ifdef DEBUG
void printBoardState(uint8_t holds[HOLD_AMOUNT])
{
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
            if (holds[ledPrintMapping[j]])
                  Serial.print("[" + String((char)(holds[ledPrintMapping[j]])) + "]");
            else
                  Serial.print("[ ]");
	}
	Serial.println();
}
#endif

void parseProblemString(String problem, uint8_t newHolds[HOLD_AMOUNT])
{
	String holdDescription = "";

	for (int i = 0; i < problem.length(); i++)
	{
		if (problem[i] == ',')
		{
                  newHolds[holdDescription.substring(1).toInt()] = holdDescription[0];
                  holdDescription = "";
		}
		else
		{
			holdDescription += problem[i];
		}
	}
      newHolds[holdDescription.substring(1).toInt()] = holdDescription[0];
      holdDescription = "";
}

void showBoard(uint8_t holds[HOLD_AMOUNT])
{
      NeoPixelBus<NeoRgbFeature, NeoWs2811Method> strip(HOLD_AMOUNT, PIN);
      strip.Begin();
      strip.Show();

      for (size_t i = 0; i < HOLD_AMOUNT; i++)
      {
            switch (holds[i])
            {
            case 'S':
                  strip.SetPixelColor(i, RgbColor(0,255,0));
                  break;
            case 'R':
            case 'L':
                  strip.SetPixelColor(i, RgbColor(0,0,255));
                  break;
            case 'E':
                  strip.SetPixelColor(i, RgbColor(255,0,0));
                  break;
            }
      }
      
      strip.Show();
}

void MoonCallback::onWrite(BLECharacteristic *pCharacteristic)
{
      String value = pCharacteristic->getValue();
      int i = 0;

      #ifdef DEBUG
      Serial.println("Recibido: " + value);
      #endif

      if (value.length() > 1 && value[0] == 'l' && value[1] == '#') // New problem
      { 
            problem = "";
            clearBoard();
            i += 2;
      }
      while (i < value.length())
      {
            if (value[i] != '#')
            {
                  problem += value[i];
            }
            else // End of string
            {
                  parseProblemString(problem, holds);
                  showBoard(holds);
                  #ifdef DEBUG
                  printBoardState(holds);
                  #endif
                  return;
            }
            i++;
      }
}