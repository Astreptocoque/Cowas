#include <Arduino.h>
#include "Encoder.h"
#include <math.h>

extern Encoder encoder;

void Encoder::begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter)
{
    pin_signal_A = _pin_signal_A;
    pin_signal_B = _pin_signal_B;
    pin_signal_Z = _pin_signal_Z;
    pulse_per_rev = _pulse_per_rev;
    diameter = _diameter;
    pinMode(pin_signal_A, INPUT);
    pinMode(pin_signal_B, INPUT);
    pinMode(pin_signal_Z, INPUT);

    // REG_PMC_PCER0 = PMC_PCER0_PID27;  // activate clock for TC0
    // REG_TC0_CMR0 = TC_CMR_TCCLKS_XC0; // select XC0 as clock source

    // // activate quadrature encoder and position measure mode, no filters
    // REG_TC0_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA;

    // // enable the clock (CLKEN=1) and reset the counter (SWTRG=1)
    // REG_TC0_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;

    rad_per_pulse = PI / pulse_per_rev;
}
void Encoder::begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter, String _ID)
{
    pin_signal_A = _pin_signal_A;
    pin_signal_B = _pin_signal_B;
    pin_signal_Z = _pin_signal_Z;
    pulse_per_rev = _pulse_per_rev;
    diameter = _diameter;
    ID = _ID;
    pinMode(pin_signal_A, INPUT);
    pinMode(pin_signal_B, INPUT);
    pinMode(pin_signal_Z, INPUT);
}
int Encoder::get_distance()
{
    return (int)(rad_per_pulse * diameter * pulses_A);
}
void Encoder::set_distance(int _distance)
{
    goal_pulses_A = _distance / (rad_per_pulse * diameter);
}

void Encoder::set_pulses_Z(encoder_direction _direction){
    if (_direction == e_up) pulses_Z++;
    else pulses_Z--;
}

int Encoder::get_pulses_A(){
    return pulses_A;
}

int Encoder::get_pulses_Z(){
    return pulses_Z;
}

encoder_direction Encoder::get_direction(){
    return direction;
}

void Encoder::step_counter()
{
    static int A_state;
    static int A_last_state = digitalRead(pin_signal_A);
    
    A_state = digitalRead(pin_signal_A);
    if (A_state != A_last_state)
    {
        if (digitalRead(pin_signal_B) != A_state)
        {
            pulses_A++;
            direction = e_down;
        }
        else
        {
            pulses_A--;
            direction = e_up;
        }
    }
    A_last_state = A_state;
}

void Encoder::reset(){
    pulses_A = 0;
    pulses_Z = 0;
}

void ISR_encoder_z_signal(){
    if(encoder.get_direction() == e_up) encoder.set_pulses_Z(e_up);
    else encoder.set_pulses_Z(e_down);
}