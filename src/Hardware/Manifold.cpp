#include <Arduino.h>
#include "Manifold.h"
#include "C_output.h"

extern C_output output;

/**
 * @brief Constructor for the manifold slots
 * @param pin_control Output connection on the board of the slot
 * @param _ID id of the manifold slot
 */
void Manifold_slot::begin(byte _pin_control, int _id)
{
    ID = _id;
    state = available;
    pin_control = _pin_control;
    //pinMode(pin_control, OUTPUT);
    if (VERBOSE_INIT){output.println("Slot manifold " + String(ID) + " initiated");}
}

int Manifold_slot::get_id()
{
    return ID;
}

void Manifold_slot::change_state(slot_state changed_state)
{
    state=changed_state;
}

slot_state Manifold_slot::get_state()
{
    return state;
}


/**
 * @brief Constructor for the manifold
 */
void Manifold::begin()
{
    for(int i=0; i < NB_SLOT; i++){
        slots[i].begin(MANIFOLD_PIN[i], i);
    }
    if (VERBOSE_INIT){output.println("Manifold initiated");}
}

int Manifold::get_id(int i)
{
    return slots[i].get_id();
}

void Manifold::change_state(int i, slot_state changed_state)
{
    slots[i].change_state(changed_state);
}

slot_state Manifold::get_state(int i)
{
    return slots[i].get_state();
}