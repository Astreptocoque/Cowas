#ifndef SERIAL_EXCEL_H
#define SERIAL_EXCEL_H

#include <Arduino.h>

class Serial_excel
{

private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    Serial_excel(byte pin);
    void init();
    void update();
    byte getState();
    bool isPressed();
};

#endif