#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

struct Timer{
    Tc *timer_clock;
    uint32_t channel;
    IRQn_Type irq;
    uint32_t frequency;
};

// void TC3_Handler();
void timerStart(struct Timer pTimer);
void timerStop(struct Timer pTimer);


#endif
