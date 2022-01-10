#ifndef ENCODER_ATMEL_H
#define ENCODER_ATMEL_H

#include <Arduino.h>
#include "interfaces/Encoder_interface.h"


class Encoder_atmel : public Encoder_interface
{

private:
    
public:
    void begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter);
    void begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter, String _ID);
    int get_distance();
    int32_t get_goal_pulses();
    int32_t get_pulses_A();
    int32_t get_pulses_B();
    int32_t get_pulses_Z();
    void set_distance_to_reach(int distance);
    encoder_direction get_direction();
    int32_t step_counter();
    void reset();
};


#endif