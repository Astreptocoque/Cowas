#ifndef ENCODER_INTERFACE_H
#define ENCODER_INTERFACE_H

#include <Arduino.h>
#include "Serial_output.h"

// include global custom output function
extern Serial_output output;

enum encoder_direction{e_down, e_up};

class Encoder_interface
{

protected:
    byte pin_signal_A; // do X pulses per round
    byte pin_signal_B; // do X puslses per round, phase shifted by 90° electric
    byte pin_signal_Z; // do a pulse each round
    int pulse_per_rev; // number of pulse in one turn for signal A and B. Datasheet.
    float rad_per_pulse; // PI divided by pulse_per_rev
    float diameter;     // diameter of spool in centimeter
    int32_t pulses_A;      // total pulse counter
    int32_t pulses_Z;      // total round counter
    int32_t pulses_B;
    int32_t goal_pulses_A;  // pulses to reach
    encoder_direction direction;
    int depth;         // depth to reach, in centimeter
    String ID = "no_ID";

public:
    virtual void begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter){
        pulse_per_rev = _pulse_per_rev;
        diameter = _diameter;
        output.println("Encoder sensor " + ID + " initiated");
    }
     virtual void begin(byte _pin_signal_A, byte _pin_signal_B, byte _pin_signal_Z, const int _pulse_per_rev, const float _diameter, String _ID){
        ID = _ID;
        pulse_per_rev = _pulse_per_rev;
        diameter = _diameter;
        output.println("Encoder sensor " + ID + " initiated");
    }
    virtual int get_distance(){
        output.println("Get linear distance from encoder");
        return 1;
    }
    virtual void set_distance_to_reach(int _distance){
        goal_pulses_A = _distance / (rad_per_pulse * diameter);
        output.println("Distance to reach for encoder " + ID + " set at " + String(_distance) + " (" + String(goal_pulses_A) + " pulses)");
    }
    virtual int32_t get_goal_pulses(){
        output.println("Get goal pulses (to reach)");
        return 1;
    }
    virtual int32_t get_pulses_A(){
        output.println("Get total pulses from signal A");
        return 1;
    }
    virtual int32_t get_pulses_B(){
        output.println("Get total pulses from signal B");
        return 1;
    }
    virtual int32_t get_pulses_Z(){
        output.println("Get total pulses from signal Z");
        return 1;
    }
    virtual void print_states(){
        output.println("Get all states from encoder");
    }
    virtual void set_pulses_Z(encoder_direction _direction){
        output.println("Set pulses of Z");
    }
    virtual encoder_direction get_direction(){
        output.println("Get direction of rotation");
        return e_down;
    }
    virtual int32_t step_counter(){
        output.println("Counting steps of encoder " + ID);
        return 1;
    }
    virtual void reset(){
        output.println("Resetted pulses count of encoder " + ID);
    }
};

#endif
