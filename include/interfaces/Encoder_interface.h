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
    int pulses_A;      // total pulse counter
    int pulses_Z;      // total round counter
    int goal_pulses_A;  // pulses to reach
    encoder_direction direction;
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
    /**
     * @brief Get the distance object
     * 
     * @return int in centimeters
     */
    virtual int get_distance(){
        output.println("Get distance from encoder");
        return 1;
    }
    /**
     * @brief Set the distance object
     * 
     * @param distance in centimeters
     */
    virtual void set_distance(int distance){
        output.println("Distance set at " + String(distance));
    }
};

#endif
