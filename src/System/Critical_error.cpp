/**
 * @file Critical_error.cpp
 * @author Timothée Hirt
 * @brief NOT IMPLEMENTED YET. Functions to handle critical errors along the program
 * @version 0.1
 * @date 2022-01-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */
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

}
