#ifndef LED_H
#define LED_H
#include <Arduino.h>
#include "interfaces/Led_interface.h"

class Led : public Led_interface
{
  
  private:
    byte control_pin;
    
  public:
    void begin(byte _control_pin);
    void on();
    void off();
};
#endif