#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

class Pump
{

private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    Pump(byte pin);
    void init();
    void update();
    byte getState();
    bool isPressed();
};

#endif