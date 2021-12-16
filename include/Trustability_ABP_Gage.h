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
    void begin(byte _pin_slave_select, float _max_pressure, String _name);
    void read();
    float getPressure();
    float getTemperature();
    
};

void ISR_pressure_checking();

#endif