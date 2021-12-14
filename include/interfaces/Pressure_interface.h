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
    String ID = "no_ID";
    float  max_pressure = 3; // in bar

public:
    virtual void begin(byte _pin_slave_select, float _max_pressure){
        output.println("Pressure sensor created on port " + _pin_slave_select);
        max_pressure = _max_pressure;
    }
    virtual void begin(byte _pin_slave_select, float _max_pressure, String _ID){
        output.println("Pressure sensor " + ID + " initiated");
        ID = _ID;
        max_pressure = _max_pressure;
    }
    virtual void read(){
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
