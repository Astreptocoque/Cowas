#include <Arduino.h>
#include "Potentiometer.h"

void Potentiometer::begin(byte _input_pin){
  input_pin = _input_pin;
  pinMode(input_pin, OUTPUT);
}

/**
 * @brief Return value from potentiometer between 0 and 255
 * 
 * @return int 
 */
int Potentiometer::get_value(){
    value = analogRead(input_pin);
    return value;
}

/**
 * @brief Return value from potentiometer and map it on a new scale (from 0 to 255 to new values)
 * 
 * @param _level_mapping_min min scale value
 * @param _level_mapping_max max scale value
 * @return int 
 */
int Potentiometer::get_value(unsigned int _level_mapping_min, unsigned int _level_mapping_max){
    value = analogRead(input_pin);
    return map(value, 0, 1023, _level_mapping_min, _level_mapping_max);
}
