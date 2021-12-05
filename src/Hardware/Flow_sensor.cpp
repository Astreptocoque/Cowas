#include <Arduino.h>
#include "Flow_sensor.h"

/**
 * @brief Constructor for a pressure sensor
 * @param pin Input connection on the board
 *
 */
Flow_sensor::Flow_sensor(byte _input_pin)
{
    input_pin = _input_pin;
    pinMode(input_pin, INPUT);
}


