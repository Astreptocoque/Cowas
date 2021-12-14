#ifndef POTENTIOMETER_INTERFACE_H
#define POTENTIOMETER_INTERFACE_H
#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

class Potentiometer_interface{
  
    private:
    byte input_pin;
    String ID = "no_ID";
    
  public:
    virtual void begin(byte _input_pin){
        input_pin = _input_pin;
        output.println("Potentiometer " + ID + " initiated");
    }
    virtual void begin(byte _input_pin, String _ID){
        input_pin = _input_pin;
        ID = _ID;
        output.println("Potentiometer " + ID + " initiated");
    }
    virtual int get_value(){
        output.println("Get potentiometer " + ID + " value");
    }
    virtual int get_value(int _level_mapping_min, int _level_mapping_max){
        output.println("Get potentiometer " + ID + " value and map it on new scale");
    }
};
#endif