#ifndef GPIO_INTERFACE_H
#define GPIO_INTERFACE_H
#include <Arduino.h>
#include "Serial_output.h"

extern Serial_output output;

enum pin_mode{input_mode, output_mode};

class GPIO_interface {
  
  protected:
    byte pin;
    byte mode;
    bool state;
    String ID = "no_ID";
    
  public:
    virtual void begin(byte _pin, byte _mode){
        output.println("GPIO " + ID + " initiated");
    }

    virtual void begin(byte _pin, byte _mode, String _ID){
        ID = _ID;
        output.println("GPIO " + ID + " initiated");
    }
    virtual void on(){
        output.println("GPIO " + ID + " ON");
    }
    virtual void off(){
        output.println("GPIO " + ID + " OFF");
    }
    virtual bool read(){
        output.println("GPIO " + ID + " read");
        return 1;
    }
};
#endif