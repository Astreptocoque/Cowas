#ifndef LED_H
#define LED_H
#include <Arduino.h>

class Led
{
private:
  byte control_pin;
  bool state;
  String ID = "no_ID";

public:
  void begin(byte _control_pin);
  void begin(byte _control_pin, String _ID);
  void on();
  void off();
  void switch_state();
};
#endif