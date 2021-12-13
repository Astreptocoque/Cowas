#include <Arduino.h>
#include "Motor.h"
#include "DualVNH5019MotorShield.h"

unsigned char INA1 = 53;     // INPUT - motor 1 direction input A
unsigned char INB1 = 51;     // INPUT - motor 1 direction input B
unsigned char PWM1 = 2;     // INPUT - motor 1 speed input
unsigned char EN1DIAG1 = 52; // Non attributed OUTPUT - motor 1 enable input/fault output
unsigned char CS1 = A11;     // Non attributed// OUTPUT - motor 1 current sense
unsigned char INA2 = 50;     // Non attributed// INPUT - motor 2 direction input A
unsigned char INB2 = 48;     // Non attributed// INPUT - motor 2 direction input B
unsigned char PWM2 = 46;     // Non attributed// INPUT - motor 2 speed input
unsigned char EN2DIAG2 = 44; // Non attributed// OUTPUT - motor 2 enable input/fault output
unsigned char CS2 = A10;     // Non attributed// OUTPUT - motor 2 current sense

DualVNH5019MotorShield md(INA1, INB1, PWM1, EN1DIAG1, CS1, INA2, INB2, PWM2, EN2DIAG2, CS2);


/**
 * @brief Constructor for a diaphragm Motor
 *
 */
void Motor::begin()
{
    md.init();
}

/**
 * @brief set speed
 * 
 * @param _speed max speed = 400, min speed = 0
 */
void Motor::set_speed(int _speed, motor_direction _direction)
{
    speed = abs(_speed);
    direction = _direction;
}

void Motor::start()
{
    md.setM1Speed(speed*(direction==up?1:-1));
    Motor_interface::start();
}

void Motor::start(uint8_t _depth)
{
}

void Motor::stop()
{

    // md.setM1Speed(0);
    md.setM1Brake(400);
    Motor_interface::stop();
}

void Motor::stopIfFault()
{
    if (md.getM1Fault())
    {
        output.println("M1 fault");
        while (1);
    }
}