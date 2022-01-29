#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H
#include <Arduino.h>

class Potentiometer
{

private:
  byte input_pin;
  int value;
  String ID = "no_ID";

public:
  void begin(byte _input_pin);
  void begin(byte _input_pin, String _ID);
  int get_value();
  int get_value(unsigned int _level_mapping_min, unsigned int _level_mapping_max);
};
#endif