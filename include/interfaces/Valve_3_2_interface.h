#ifndef VALVE_3_2_INTERFACE_H
#define VALVE_3_2_INTERFACE_H

#include "Serial_output.h"
#include "Valve_interface.h"

extern Serial_output output;

enum valve_3_2_state{L_way, I_way};

class Valve_3_2_interface : public Valve_interface
{

protected:
    int pin_slave_select;
    valve_3_2_state state; // 1 open, 0 closed

public:
    virtual void begin(byte _pin_control)
    {
        output.println("Valve created on port " + _pin_control);
        state = I_way;
        output.println("Valve init I way");
    }

    virtual void switch_way()
    {
        output.println("Valve on " + String((get_state()?"I":"L")) + " way");
        if(state == L_way) state = I_way; 
        else state = L_way;
    }

    virtual void set_L_way()
    {
        state = L_way;
        output.println("Valve on L way - TO CHECK");
    }
    virtual void set_I_way()
    {
        state = I_way;
        output.println("Valve on I way - TO CHECK");
    }

    virtual valve_3_2_state get_state()
    {
        output.println("Get valve state");
        return state;
    }
};

#endif
