#pragma once
#include <array>
#include <string>
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
      Problem(const std::string &problemString);

      static void parseProblemString(const std::string &problemString, std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> *outHolds);

      std::array<Hold::HOLDTYPE_t, HOLD_AMOUNT> getHolds() const;
      char getConfiguration() const;

      void process(void);
      void printBoardState() const;
};