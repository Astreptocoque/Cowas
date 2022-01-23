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
    uint32_t last_reading = 0;

public:
    virtual void begin(byte _pin_slave_select, float _max_pressure){
        max_pressure = _max_pressure;
        output.println("Pressure sensor " + ID + " initiated");

    }
    virtual void begin(byte _pin_slave_select, float _max_pressure, String _ID){
        ID = _ID;
        max_pressure = _max_pressure;
        output.println("Pressure sensor " + ID + " initiated");
    }
    virtual void read(){
        output.println("Pressure from sensor " + ID + " read");
    }
    virtual float getPressure(){
        output.println("Pressure from sensor " + ID + " fetched");
        return 1;
    }
    virtual float getTemperature(){
        output.println("Temperature from sensor " + ID + " fetched");
        return temperature;
    }
    virtual float getMaxPressure(){
        output.println("Pressure max from sensor " + ID + " is " + max_pressure + " bar");
        return max_pressure;
    }
};

#endif
