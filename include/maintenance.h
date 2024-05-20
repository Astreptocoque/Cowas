#ifndef MAINTENANCE_H
#define MAINTENANCE_H

#include <Arduino.h>

#define MAINT_FILL_TIME 30000
#define MAINT_PURGE_TIME 30000

/**
    @brief: Disconnect the deployment module and put the connection into ethanol
*/
void purge_pipes_manifold();


#endif