#ifndef MANIFOLD_H
#define MANIFOLD_H

#include <Arduino.h>
#include "C_output.h"

enum slot_state
{
    unaivailable,
    available
};

class Manifold_slot{
    private:
        int pin_control;
        slot_state state; // 1 available, 0 unaivailable
        int ID;

    public:
        void begin(byte _pin_control, int _id);
        void change_state(slot_state state);
        slot_state get_state();
        int get_id();
};


class Manifold{
    private:
        Manifold_slot slots[NB_SLOT];
    public:
        void begin();
        void change_state(int i, slot_state state);
        slot_state get_state(int i);
        int get_id(int i);
};

#endif