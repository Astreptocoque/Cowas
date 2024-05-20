#ifndef STEP_FUNCTION_H
#define STEP_FUNCTION_H

#include <Arduino.h>

void step_dive(int _depth);
void step_purge();
void step_fill_container();
void step_sampling(int slot_manifold);
void step_rewind();
void step_dry(uint8_t num_sterivex);
void sample_process(int depth);
void purge_sterivex(int valve);
void purge_Pipes();

void demo_sample_process();

#endif