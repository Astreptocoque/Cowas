#ifndef TESTS_H
#define TESTS_H

#include <Arduino.h>


void test_all_components();

void test_valves();
void test_motor_spool();
void test_pump();
void test_command_box();
void test_micro_switch();
void test_encoder();
void test_encoder_spool();

void test_1_depth_20m();
void test_1_depth_40m();
void test_1_depth_40m_direct();

void test_2_remplissage_container_1m();
void test_2_remplissage_container_40m();

void test_3_sterivex_1();
void test_3_sterivex_2();

void tests();
void test_pressure();


void test_pressure_sensor();

void test_manifold();

void add_button_manifold_demo();


#endif