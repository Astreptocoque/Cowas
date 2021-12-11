#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <interfaces/Button_interface.h>

class Button : public Button_interface
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