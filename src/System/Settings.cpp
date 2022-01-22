#include "Settings.h"
#include "Serial_output.h"

extern Serial_output output;

uint8_t FILTER_IN_SYSTEM = 2;           // max samples currently inserted in the system

System_state system_state = state_starting;

System_state get_system_state(){
    return system_state;
}

void set_system_state(System_state state){
    system_state = state;
    // TODO : communicate state to wifi card
}
