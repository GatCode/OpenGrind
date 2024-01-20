#pragma once
#include "Definitions.h"

class Dosage
{
  private:
    int eeAddress = 0;

  public:
    Dosage();
    bool singleDoseSelected = false;
    bool manualModeSelected = false;
    double singleDoseTime = SINGLEDOSE_DEFAULT_TIME;
    double doubleDoseTime = DOUBLEDOSE_DEFAULT_TIME;
    void writeToEEPROM();
};