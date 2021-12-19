#include <Arduino.h>
#include "Motor.h"
#include "DualVNH5019MotorShield.h"
#include "Encoder.h"
#include "Settings.h"
#include "Button.h"
#include "Critical_error.h"

unsigned char INA1 = 53;     // INPUT - motor 1 direction input A
unsigned char INB1 = 51;     // INPUT - motor 1 direction input B
unsigned char PWM1 = 2;      // INPUT - motor 1 speed input
unsigned char EN1DIAG1 = 49; // Non attributed OUTPUT - motor 1 enable input/fault output
unsigned char CS1 = A11;     // Non attributed// OUTPUT - motor 1 current sense
unsigned char INA2 = 52;     // Non attributed// INPUT - motor 2 direction input A
unsigned char INB2 = 50;     // Non attributed// INPUT - motor 2 direction input B
unsigned char PWM2 = 48;     // Non attributed// INPUT - motor 2 speed input
unsigned char EN2DIAG2 = 47; // Non attributed// OUTPUT - motor 2 enable input/fault output
unsigned char CS2 = A10;     // Non attributed// OUTPUT - motor 2 current sense

DualVNH5019MotorShield md(INA1, INB1, PWM1, EN1DIAG1, CS1, INA2, INB2, PWM2, EN2DIAG2, CS2);
extern Encoder encoder;
extern Motor spool;
extern Button button_spool_up;
extern Button button_spool_down;

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
 * @param _speed max speed = 100, min speed = 0
 */
void Motor::set_speed(int _speed, motor_direction _direction)
{
    if (_speed > 100)
        _speed = 100;
    if (_speed < 0)
        _speed = 0;
    speed = map(_speed, 0, 100, 0, 400);
    direction = _direction;
}

void Motor::start()
{
    // security at end stop
    if ((direction == up && endstop_up) || (direction == down && endstop_down))
    {
        output.println("Reach end of tube, order cancelled | cannot wind up more");
    }
    else
    {
        endstop_up = false;
        endstop_down = false;
        // set here direction of motor
        md.setM1Speed(speed * (direction == up ? 1 : -1));
        Motor_interface::start();
    }
}

/**
 * @brief user manual function
 */
void Motor::start(int _speed, motor_direction _direction)
{
    set_speed(_speed, _direction);
    start();
}

/**
 * @brief go to a given depth
 *        negative number to go out of water
 *
 * @param _depth in centimeters
 */
void Motor::start(int _depth)
{

    // if complete rewind, set a security distance to stop, then use start_origin function
    if (_depth <= 0)
        _depth = -HEIGHT_FROM_WATER + DISTANCE_FROM_STOP;
    if(_depth > TUBE_LENGTH)
        _depth = -HEIGHT_FROM_WATER + TUBE_LENGTH;

    depth_goal = _depth;                           // distance from water level
    int distance = depth_goal + HEIGHT_FROM_WATER; // absolute distance from sensor
    encoder.set_distance_to_reach(distance);

    // movement setup depending of absolute depth
    if (depth_goal > depth_current)
        set_speed(SPEED_DOWN, down);
    else if (depth_goal < depth_current)
        set_speed(SPEED_UP, up);
    else
        set_speed(0, down);

    Motor_interface::start(_depth);

    // INFO | function can be accelerated and made more precises ATMEL hardware encoder core

    start();
    // loop take ~6 us.
    if (direction == down)
        while (encoder.get_distance() < distance)
            encoder.step_counter();
    else
        while (encoder.get_distance() > distance)
            encoder.step_counter();
    stop();


    // update state
    depth_current = depth_goal;
    if (_depth <= 0)
        start_origin();
}

/**
 * @brief reset the motor to initial position at low speed using microswitch sensor
 *
 */
void Motor::start_origin()
{
    // check that sensor is working
    if (button_spool_up.getState() == 1)
    {
        
        set_speed(30, up);

        Motor_interface::start_origin();

        start();
        while (button_spool_up.getState() == 1)
            ;
        stop(); // should be useless as interrupt is taking care of stopping the motor
        encoder.reset();
        depth_current = -HEIGHT_FROM_WATER;
    }
    else
    {
        output.println("Error with spool button | sensor not working");
        critical_error(1);
    }
}

void Motor::stop()
{
    md.setM1Brake(300);
    Motor_interface::stop();
}

void Motor::stopIfFault()
{
    if (md.getM1Fault())
    {
        output.println("M1 fault");
        while (1)
            ;
    }
}

/**
 * @brief Interrupt function
 * Do not serial print here, cause crash
 *
 */
void ISR_emergency_stop_up()
{
    if (spool.direction == up && spool.endstop_up == false)
    {
        spool.endstop_up = true;
        md.setM1Brake(400);
        output.println("Endstop up touched");
    }
}

/**
 * @brief Interrupt function
 * Do not serial print here, cause crash
 *
 */
void ISR_emergency_stop_down()
{
    if (spool.direction == down && spool.endstop_down == false)
    {
        spool.endstop_down = true;
        md.setM1Brake(400);
        output.println("Endstop down touched");
    }
}