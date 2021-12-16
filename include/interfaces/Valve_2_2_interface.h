#ifndef VALVE_2_2_INTERFACE_H
#define VALVE_2_2_INTERFACE_H

#include "Serial_output.h"

extern Serial_output output;

enum valve_2_2_state{open_way, close_way};

class Valve_2_2_interface
{

protected:
    int pin_control;
    valve_2_2_state state; // 1 open, 0 closed
    String ID = "no_ID";

public:
    virtual void begin(byte _pin_control, String _ID)
    {
        ID = _ID;
        output.println("Valve " + ID + " initated");
        set_close_way();
    }

    virtual void begin(byte _pin_control)
    {
        output.println("Valve " + ID + " initated");
        set_open_way();
    }

    virtual void switch_way()
    {
        if(state == open_way) state = close_way;
        else state = open_way;
        output.println("Valve " +ID +  String((state == open_way?" open":" close")));
    }

    virtual void set_open_way()
    {
        state = open_way;
        output.println("Valve " + ID + " opened");
    }
    virtual void set_close_way()
    {
        state = close_way;
        output.println("Valve " + ID + " closed");
    }

    virtual valve_2_2_state get_state()
    {
        output.println("Get valve " + ID + " state");
        return state;
    }
};

#endif
