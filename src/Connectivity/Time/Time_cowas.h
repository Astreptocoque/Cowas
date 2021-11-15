#ifndef TIME_COWAS_H
#define TIME_COWAS_H

#include <Arduino.h>

class Time_cowas
{

private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    Time_cowas(byte pin);
    void init();
    void update();
    byte getState();
    bool isPressed();
};

#endif