#include <Arduino.h>
#include "Potentiometer.h"
#include "C_output.h"

extern C_output output;

void Potentiometer::begin(byte _input_pin)
{
  input_pin = _input_pin;
  pinMode(input_pin, OUTPUT);
  output.println("Potentiometer " + ID + " initiated");
}

void Potentiometer::begin(byte _input_pin, String _ID)
{
  ID = _ID;
  begin(_input_pin);
}

/**
 * @brief Return value from potentiometer
 *
 * @return int between 0 and 1023
 */
int Potentiometer::get_value()
{
  value = analogRead(input_pin);
  return value;
}

/**
 * @brief Return value from potentiometer and map it on a new scale (from 0 to 1023 to new values)
 *
 * @param _level_mapping_min min scale value
 * @param _level_mapping_max max scale value
 * @return int
 */
int Potentiometer::get_value(unsigned int _level_mapping_min, unsigned int _level_mapping_max)
{
  value = analogRead(input_pin);
  return map(value, 0, 1023, _level_mapping_min, _level_mapping_max);
}
