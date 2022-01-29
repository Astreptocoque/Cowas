#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H
#include <Arduino.h>
#include "interfaces/Potentiometer_interface.h"

class Potentiometer : public Potentiometer_interface
{
  
  private:
  public:
    void begin(byte _input_pin);
    void begin(byte _input_pin, String _ID);
    int get_value();
    int get_value(unsigned int _level_mapping_min, unsigned int _level_mapping_max);
};
#endif