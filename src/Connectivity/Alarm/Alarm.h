#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>

class Alarm
{

private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    Alarm(byte pin);
    void init();
    void update();
    byte getState();
    bool isPressed();
};

#endif