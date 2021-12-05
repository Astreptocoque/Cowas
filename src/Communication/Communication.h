#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include "Settings.h"

enum messages{
    cowas_busy = 0,
    cowas_available,
    get_time,

};

class Communication{
    struct Date get_time();

};

#endif