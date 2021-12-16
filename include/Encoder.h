#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>
#include "interfaces/Encoder_interface.h"


class Encoder : public Encoder_interface
{

private:
    
public:
    void begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter);
    void begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter, String _ID);
    int get_distance();
    int get_pulses_A();
    int get_pulses_Z();
    void set_distance(int distance);
    void set_pulses_Z(encoder_direction _direction);
    encoder_direction get_direction();
    void step_counter();
    void reset();
};


void ISR_encoder_z_signal();

#endif