#ifndef Pressure_Interface_h
#define Pressure_Interface_h

#include <Arduino.h>
#include "Serial_output.h"

// include global custom output function
extern Serial_output output;

class Pressure_interface
{

protected:
    int pin_slave_select;
    float pressure = 0;
    float temperature = 0;

public:

    Pressure_interface(){
        output.println("Pressure sensor initialized without port");
    }

    Pressure_interface(byte _pin_slave_select){
        output.println("Pressure sensor created on port " + _pin_slave_select);
    }

    virtual void readPressure(){
        output.println("Pressure read");
    }
    virtual float getPressure(){
        output.println("Pressure fetched");
        return pressure;
    }
    virtual float getTemperature(){
        output.println("Temperature fetched");
        return temperature;
    }
};

#endif
