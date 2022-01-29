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

    void update();

public:
    void begin(byte _input_pin);
    void begin(byte _input_pin, String _ID);
    byte getState();
    bool isPressed();
    bool isReleased();
    void waitPressedAndReleased();
    void waitReleasedAndPressed();
};

#endif