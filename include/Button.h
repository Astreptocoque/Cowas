#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{

private:
    byte input_pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    void begin(byte _input_pin);
    void update();
    byte getState();
    bool isPressed();
    void waitPressedAndReleased();
};

#endif