#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>

class Flow_sensor
{

private:
    byte input_pin;


public:
    Flow_sensor(byte pin);

};

#endif