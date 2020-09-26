#include <Grinder.h>
#include <Arduino.h>
#include "Definitions.h"
#include <EEPROM.h>

Grinder::Grinder() {
    pinMode(SSR, OUTPUT);
    pinMode(START_BTN, INPUT_PULLUP);
}

bool Grinder::startBtnPressed() {
    return !digitalRead(START_BTN); // Negated because PULLUP
}

int Grinder::getSingleDoseStats() {
    static int res = 0;
    EEPROM.get(eeAddress, res);
    return res;
}

int Grinder::getDoubleDoseStats() {
    static int res = 0;
    EEPROM.get(eeAddress + sizeof(int), res);
    return res;
}

void Grinder::increaseShotCounter(bool isSingleDose) {
    if (isSingleDose) {
        EEPROM.put(eeAddress, getSingleDoseStats() + 1);
    } else {
        EEPROM.put(eeAddress + sizeof(int), getDoubleDoseStats() + 1);
    }
}

void Grinder::resetStats() {
    for (int i = 0 ; i < EEPROM.length() ; i++) {
        EEPROM.write(i, 0);
    }
}

unsigned long Grinder::getTargetTime() {
    return targetTime;
}

void Grinder::on(double targetTime) {
    this->targetTime = millis() + targetTime * 1000;
    digitalWrite(SSR, HIGH);
}

void Grinder::off() {
    digitalWrite(SSR, LOW);
}