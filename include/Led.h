#ifndef LED_H
#define LED_H
#include <Arduino.h>
#include "interfaces/Led_interface.h"

class Led : public Led_interface
{
  public:
    void begin(byte _control_pin);
    void begin(byte _control_pin, String _ID);
    void on();
    void off();
    void switch_state();
};
#endif