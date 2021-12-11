#ifndef VALVE_3_2_INTERFACE_H
#define VALVE_3_2_INTERFACE_H

#include "Serial_output.h"

extern Serial_output output;

enum valve_3_2_state{L_way, I_way};

class Valve_3_2_interface
{

protected:
    int pin_control;
    valve_3_2_state state; // 1 open, 0 closed
    String name = "default";

public:
    virtual void begin(byte _pin_control, String _name)
    {
        name = _name;
        output.println("Valve " + name+ " created on port " + _pin_control);
        state = I_way;
        output.println("Valve init I way");
    }

    virtual void begin(byte _pin_control)
    {
        output.println("Valve " + name+ " created on port " + _pin_control);
        state = I_way;
        output.println("Valve init I way");
    }

    virtual void switch_way()
    {
        if(state == L_way) state = I_way; 
        else state = L_way;
        output.println("Valve " +name+ " on " + String((state == I_way?"I":"L")) + " way");

    }

    virtual void set_L_way()
    {
        state = L_way;
        output.println("Valve " + name + " on L way");
    }
    virtual void set_I_way()
    {
        state = I_way;
        output.println("Valve " + name + " on I way");
    }

    // virtual valve_3_2_state get_state()
    // {
    //     output.println("Get valve state");
    //     return state;
    // }
};

#endif
