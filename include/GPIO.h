#ifndef GPIO_H
#define GPIO_H
#include <Arduino.h>
#include "interfaces/GPIO_interface.h"

class GPIO : public GPIO_interface
{
  public:
    void begin(byte _pin, byte _mode);
    void begin(byte _pin, byte _mode, String _ID);
    void on();
    void off();
    byte read();
};
#endif