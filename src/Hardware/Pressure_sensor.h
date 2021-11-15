#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include <Arduino.h>
//#include "Interfaces/Pressure_Interface.h"
#include <SPI.h>

class Pressure_sensor
{

private:
    byte pin_slave_select;
    float pressure;
    float temperature;
    // SPISettings* SPIPressure;

public:
    Pressure_sensor(byte _pin_slave_select);
    void readPressure();
    float getPressure();
    float getTemperature();
};

#endif