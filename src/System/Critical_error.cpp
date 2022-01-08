#include <Arduino.h>
#include "Serial_output.h"
#include "Settings.h"
#include "Critical_error.h"

extern Serial_output output;

void critical_error(unsigned int code){

    switch(code){
        case 1:
            output.println("Error sensor not working");
        break;
        default:
            output.println("Error code not known");
    }

    // log date and time
    // output.println("Date : " + String(current_date.day) + "." + String(current_date.month) + "." + String(current_date.year) + " "
    //                + String(current_date.time.hour) + "." + String(current_date.time.minutes));

    // reset the arduino with error code
    // TODO store error code in sd card
/*     resetFunc();
 */}
