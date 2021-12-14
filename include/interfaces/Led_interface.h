#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H
#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

class Led_interface {
  
  private:
    byte control_pin;
    String ID = "no_ID";
    
  public:
    virtual void begin(byte _control_pin){
        control_pin = _control_pin;
        output.println("LED " + ID + " initiated");
    }

    virtual void begin(byte _control_pin, String _ID){
        control_pin = _control_pin;
        ID = _ID;
        output.println("LED " + ID + " initiated on pin");
    }
    virtual void on(){
        output.println("LED " + ID + " ON");
    }
    virtual void off(){
        output.println("LED " + ID + " OFF");
    }
};
#endif