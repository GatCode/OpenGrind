#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include "RotaryEncoder.h"
#include "Definitions.h"
#include "Display.h"
#include "Dosage.h"
#include "Grinder.h"

RotaryEncoder *encoder;
Display * display;
Dosage *dosage;
Grinder *grinder;

// State Machine
enum States {NORMAL, SET_DOSE, GRINDING, STATS};
// Modes
enum Modes {SINGLE, DOUBLE, MANUAL};

uint8_t state = NORMAL;
uint8_t mode = DOUBLE;
double manualGrindTime =0.0;


void setup() {
  Serial.begin(9600);
  encoder = new RotaryEncoder();
  display = new Display();
  dosage = new Dosage();
  grinder = new Grinder();
}

void(* resetFunc) (void) = 0;

void loop() {
  switch(state)
  {
    case NORMAL:
    grinder->off();
      // set new dose
      if (encoder->wasPressed()) {
        state = SET_DOSE;
        break;
      }

      // start grinding
      if (grinder->startBtnPressed()) {
        state = GRINDING;
        break;
      }

      // show stats
      if (encoder->wasLongPressed()) {
        state = STATS;
        break;
      }
      
      switch(mode){
        case(SINGLE):
          if(encoder->wasTurnedRight())mode=DOUBLE;
          break;
        case(DOUBLE):
          if(encoder->wasTurnedLeft())mode=SINGLE;
          if(encoder->wasTurnedRight())mode=MANUAL;
          break;
        case(MANUAL):
          if(encoder->wasTurnedLeft())mode=DOUBLE;
          break;
      }
      

      // display dosage
      if (mode==SINGLE) {
        dosage->singleDoseSelected=true;
        dosage->manualModeSelected=false;
        manualGrindTime=0.0;
        display->printSingleDose();
      } else if(mode==DOUBLE) {
        dosage->singleDoseSelected=false;
        dosage->manualModeSelected=false;
        manualGrindTime=0.0;
        display->printDoubleDose() ;
      } else if(mode==MANUAL) {
        dosage->singleDoseSelected=false;
        dosage->manualModeSelected=true;
        display->printManualMode(manualGrindTime*1.0);
      }
        

      break;

    case SET_DOSE:
      if(dosage->manualModeSelected==false){
        if (encoder->wasTurnedLeft()) {
          dosage->singleDoseTime -= dosage->singleDoseSelected ? 0.1 : 0.0;
          dosage->doubleDoseTime -= dosage->singleDoseSelected==false ? 0.1 : 0.0;
        } else if (encoder->wasTurnedRight()) {
          dosage->singleDoseTime += dosage->singleDoseSelected ? 0.1 : 0.0;
          dosage->doubleDoseTime += dosage->singleDoseSelected==false ? 0.1 : 0.0;
        }
        dosage->writeToEEPROM();
        display->printTime(dosage->singleDoseSelected ? dosage->singleDoseTime : dosage->doubleDoseTime);
      }
      
      state = NORMAL;
      break;

    case GRINDING:
      if(dosage->manualModeSelected){
        int startTime = millis()/1000.0;
        while(grinder->startBtnPressed()){
          grinder->on(1);
          display->printTime(millis()/1000.0-startTime);

        }
        manualGrindTime+=millis()/1000.0-startTime;
        grinder->off();
        state = NORMAL;
        break;
      }else

      grinder->increaseShotCounter(dosage->singleDoseSelected);

      grinder->on(dosage->singleDoseSelected ? dosage->singleDoseTime : dosage->doubleDoseTime);
      while (millis() < grinder->getTargetTime()) {
        display->printTime((grinder->getTargetTime() - millis()) / 1000.0);
      }
      grinder->off();

      display->printTime(0.0);
      delay(500); // show 0.0 on display for a longer time

      state = NORMAL;
      break;
 
    case STATS:
      display->printStatistics(grinder->getSingleDoseStats(), grinder->getDoubleDoseStats());

      unsigned long target = millis() + ENC_SW_LONG_PRESS_DUR;
      while (encoder->isPressed())
      {
        if (millis() > target) {
          grinder->resetStats();
          resetFunc();
        }
      }

      break;
  }
}