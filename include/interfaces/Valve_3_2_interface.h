#ifndef VALVE_3_2_INTERFACE_H
#define VALVE_3_2_INTERFACE_H

#include "Serial_output.h"

extern Serial_output output;

enum valve_3_2_state{L_way, I_way};

class Valve_3_2_interface
{

protected:
    int pin_control;
    valve_3_2_state state; // 1 I way, 0 L way
    String ID = "no_ID";

public:
    virtual void begin(byte _pin_control)
    {
        output.println("Valve " + ID + " initiated");
        set_I_way();
    }

    virtual void begin(byte _pin_control, String _ID)
    {
        ID = _ID;
        begin(_pin_control);
    }

    virtual void switch_way()
    {
        output.println("Valve " + ID + " on " + String((state == I_way?"I":"L")) + " way");
    }

    virtual void set_L_way()
    {
        output.println("Valve " + ID + " on L way");
    }
    
    virtual void set_I_way()
    {
        output.println("Valve " + ID + " on I way");
    }

    virtual valve_3_2_state get_state()
    {
        output.println("Get valve " + ID + " state");
        return state;
    }
};

#endif
