/**
 * @file Encoder_atmel.cpp
 * @author Timothée Hirt & Paco Mermoud
 * @brief Use of hardware encoder of cortex M3. Not implemented.
 * @version 0.1
 * @date 2022-01-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include "Encoder_atmel.h"
#include <math.h>


void Encoder_atmel::begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter)
{
    pin_signal_A = digitalPinToBitMask(_pin_signal_A);
    pin_signal_B = digitalPinToBitMask(_pin_signal_B);
    pin_signal_Z = digitalPinToBitMask(_pin_signal_Z);
    pulse_per_rev = _pulse_per_rev;
    diameter = _diameter;


    // https://forum.arduino.cc/t/using-the-hardware-quadrature-Encoder_atmel-channel-on-the-due/136904/10
    // activate peripheral functions for quad pins
    REG_PIOB_PDR = pin_signal_A;     // activate peripheral function (disables all PIO functionality)
    REG_PIOB_ABSR |= pin_signal_A;   // choose peripheral option B    
    REG_PIOB_PDR = pin_signal_B;     // activate peripheral function (disables all PIO functionality)
    REG_PIOB_ABSR |= pin_signal_B;   // choose peripheral option B 

    // activate clock for TC0
    REG_PMC_PCER0 = PMC_PCER0_PID27;
    // select XC0 as clock source
    REG_TC0_CMR0 = TC_CMR_TCCLKS_XC0; 
    // activate quadrature Encoder_atmel and position measure mode, no filters
    REG_TC0_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA;
    // enable the clock (CLKEN=1) and reset the counter (SWTRG=1)
    REG_TC0_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;

    rad_per_pulse = PI / pulse_per_rev;
}
void Encoder_atmel::begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter, String _ID)
{
    ID = _ID;
    begin(_pin_signal_A, _pin_signal_B, _pin_signal_Z, _pulse_per_rev, _diameter);
}

int Encoder_atmel::get_distance()
{
    return (int)(rad_per_pulse * diameter * pulses_A);
}

/**
 * @brief set distance in pulses to reach
 * 
 * @param _distance in centimeters
 */
void Encoder_atmel::set_distance_to_reach(int _distance)
{
    goal_pulses_A = _distance / (rad_per_pulse * diameter);
}

int32_t Encoder_atmel::get_pulses_A()
{
    return pulses_A;
}

int32_t Encoder_atmel::get_pulses_Z()
{
    return pulses_Z;
}

int32_t Encoder_atmel::get_pulses_B()
{
    return pulses_B;
}

encoder_direction Encoder_atmel::get_direction()
{
    return direction;
}
int32_t Encoder_atmel::get_goal_pulses()
{
    return goal_pulses_A;
}

int32_t Encoder_atmel::step_counter()
{
    pulses_A = (uint32_t)(REG_TC0_CV0);
    return pulses_A;
}

void Encoder_atmel::reset()
{
   
}