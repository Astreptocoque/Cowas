#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H
#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

class Led_interface {
  
  private:
    byte control_pin;
    
  public:
    virtual void begin(byte _control_pin){
        control_pin = _control_pin;
        output.println("LED created on pin " + String(control_pin));
    }
    virtual void on(){
        output.println("LED pin " + String(control_pin) + " ON");
    }
    virtual void off(){
        output.println("LED pin " + String(control_pin) + " OFF");
    }
};
#endif