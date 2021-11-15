#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>

class Flow_sensor
{

private:
    byte pin;
    byte state;
    byte lastReading;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

public:
    Flow_sensor(byte pin);

    void init();
    void update();
    byte getState();
    bool isPressed();
};

#endif