#ifndef SERIAL_TERMINAL_H
#define SERIAL_TERMINAL_H

#include <Arduino.h>

class Serial_terminal
{

private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    Serial_terminal(byte pin);
    void init();
    void update();
    byte getState();
    bool isPressed();
};

#endif