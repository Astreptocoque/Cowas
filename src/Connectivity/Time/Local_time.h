#ifndef LOCAL_TIME_H
#define LOCAL_TIME_H

#include <Arduino.h>

class Local_time
{

private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    Local_time(byte pin);
    void init();
    void update();
    byte getState();
    bool isPressed();
};

#endif