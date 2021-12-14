#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H
#include <Arduino.h>
#include "interfaces/Potentiometer_interface.h"

class Potentiometer : public Potentiometer_interface
{
  
  private:
    
  public:
    void begin(byte _input_pin);
    int get_value();
    int get_value(int _level_mapping_min, int _level_mapping_max);
};
#endif