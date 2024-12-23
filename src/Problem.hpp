#pragma once
#include <array>
#include "Hold.hpp"
#include "defines.hpp"

class Problem
{
      std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> m_holds;
      char m_configuration;
      std::string m_problemString;

      void setAdditionalLeds();

public:
      Problem(void);
      Problem(const std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> &holds, const char configuration);
      Problem(std::string problemString);

      static void parseProblemString(const std::string &problemString, std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> *outHolds);

      void process(void);
      void printBoardState();
};