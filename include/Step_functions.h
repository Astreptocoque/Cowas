#ifndef STEP_FUNCTION_H
#define STEP_FUNCTION_H

#include <Arduino.h>

void step_dive(int _depth);
void step_purge(bool stop_pressure = true);
void step_fill_container();
void step_sampling(int slot_manifold, bool stop_pressure = true);
void step_rewind();
void step_dry(uint8_t num_sterivex);
void sample_process(int depth, int manifold_slot = -1);
void purge_sterivex(int slot_manifold);
void purge_Pipes();
void step_DNA_shield(int slot_manifold);

void demo_sample_process();

#endif