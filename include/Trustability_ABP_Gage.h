#ifndef TRUSTABILITY_ABP_GAGE_H
#define TRUSTABILITY_ABP_GAGE_H

#include <Arduino.h>
#include "interfaces/Pressure_interface.h"

#include <SPI.h>


class Trustability_ABP_Gage : public Pressure_interface
{

private:

public:
    void begin(byte _pin_slave_select, float _max_pressure);
    void begin(byte _pin_slave_select, String _name, float _max_pressure);
    void read();
    float getPressure();
    float getTemperature();
};

#endif