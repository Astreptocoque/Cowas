#include <Arduino.h>
#include "Settings.h"
#include "Serial_output.h"

extern Serial_output output;

uint8_t FILTER_IN_SYSTEM = 2;           // max samples currently inserted in the system
bool ENABLE_OUTPUT = true;

System_state system_state = state_starting;

System_state get_system_state(){
    return system_state;
}

void set_system_state(System_state state){
    system_state = state;
    // TODO : communicate state to wifi card and 
   /*  switch (state){
        case state_starting:

        break;
        case state_idle:
        
        break;
        case state_sampling:
        
        break;
        case state_refill:
        
        break;
        case state_communicating:
        
        break;
        case state_error:
        
        break;
    } */
}

String format_date_logging(time_t t){
    // add time before printing in format yyyy-mm-dd hh:mm:ss
    String data = String(year(t))+"-";
    if(month(t) < 10)
        data += String("0")+String(month(t));
    else
        data += String(month(t));
    data+="-";
    if(day(t) < 10)
        data += String("0")+String(day(t));
    else
        data += String(day(t));
    data += " ";
    if(hour(t) < 10)
        data += String("0")+String(hour(t));
    else
        data += String(hour(t));
    data+=":";
    if(minute(t) < 10)
        data += String("0")+String(minute(t));
    else
        data += String(minute(t));
    data+=":";
    if(second(t) < 10)
        data += String("0")+String(second(t));
    else
        data += String(second(t));
    return data;
}

String format_date_friendly(time_t t){
    // add time before printing in format hh:mm:ss yyyy-mm-dd
    
    String data = "";
    if(hour(t) < 10)
        data += String("0")+String(hour(t));
    else
        data += String(hour(t));
    data+="h";
    if(minute(t) < 10)
        data += String("0")+String(minute(t));
    else
        data += String(minute(t));
    data+="m";
    data+= " " + String(year(t))+"-";
    if(month(t) < 10)
        data += String("0")+String(month(t));
    else
        data += String(month(t));
    data+="-";
    if(day(t) < 10)
        data += String("0")+String(day(t));
    else
        data += String(day(t));
    return data;
}
