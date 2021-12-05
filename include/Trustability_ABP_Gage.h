#ifndef TRUSTABILITY_ABP_GAGE_H
#define TRUSTABILITY_ABP_GAGE_H

#include <Arduino.h>
#include "Pressure_interface.h"

#include <SPI.h>


class Trustability_ABP_Gage : public Pressure_interface
{

private:

public:
    Trustability_ABP_Gage(byte _pin_slave_select);
    void readPressure();
    float getPressure();
    float getTemperature();
};

#endif