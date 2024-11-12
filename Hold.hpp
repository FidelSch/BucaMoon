#pragma once

namespace Hold
{
      typedef enum
      {
            START_HOLD = 'S',
            PROGRESS_HOLD = 'P',
            END_HOLD = 'E',
            RIGHT_HOLD = 'R',
            LEFT_HOLD = 'L',
            MATCH_HOLD = 'M',
            FOOT_HOLD = 'F',
            ADDITIONAL_LED = 'A',
            NO_HOLD = (char)0
      } HOLDTYPE_t;
};