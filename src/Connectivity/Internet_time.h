#ifndef INTERNET_TIME
#define INTERNET_TIME

#include <Arduino.h>

class Internet_time
{

private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    Internet_time
(byte pin);
    void init();
    void update();
    byte getState();
    bool isPressed();
};

#endif