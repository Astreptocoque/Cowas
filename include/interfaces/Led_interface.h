#ifndef LED_INTERFACE_H
#define LED_INTERFACE_H
#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

class Led_interface {
  
  protected:
    byte control_pin;
    bool state;
    String ID = "no_ID";
    
  public:
    virtual void begin(byte _control_pin){
        output.println("LED " + ID + " initiated");
    }

    virtual void begin(byte _control_pin, String _ID){
        ID = _ID;
        output.println("LED " + ID + " initiated");
    }
    virtual void on(){
        output.println("LED " + ID + " ON");
    }
    virtual void off(){
        output.println("LED " + ID + " OFF");
    }
    virtual void switch_state(){
        output.println("LED " + ID + " switched");

        }
};
#endif