/**
 * @file Flow_sensor.cpp
 * @author Timothée Hirt & Paco Mermoud
 * @brief Not implemented
 * @version 0.1
 * @date 2022-01-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include "Flow_sensor.h"

/**
 * @brief Not implemented.
 * @param pin Input connection on the board
 *
 */
void Flow_sensor::begin(byte _input_pin)
{
    input_pin = _input_pin;
    pinMode(input_pin, INPUT);
}


