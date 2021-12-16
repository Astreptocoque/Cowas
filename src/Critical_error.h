#ifndef CRITICAL_ERRORS_H
#define CRITICAL_ERRORS_H

#include <Arduino.h>
#include "Serial_output.h"
#include "Settings.h"

// reset function @ Saddress 0
/* static void(* resetFunc)(void) = 0;
 */
void critical_error(unsigned int code);


#endif