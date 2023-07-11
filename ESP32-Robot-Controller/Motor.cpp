#include "Motor.h"

team967::Motor::Motor()
{
}

team967::Motor::Motor(int id, int pin)
{
  this->id = id;
  this->pin = pin;
}

void team967::Motor::setMotor() {
  servo.setPeriodHertz(50);
  servo.attach(pin, 500, 2400);
}

void team967::Motor::setPower(float power)
{
  //1000 - 1500 - 2000 PWM limits for low - center - high throttle
  int microseconds = (500 * power) + 1500;
  servo.write(microseconds);
}