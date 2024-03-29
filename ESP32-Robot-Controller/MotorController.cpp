/**************************************************************************************************************************************

Author(s): Nathan Stark

Created: 5-15-2021

Last Updated: 6-21-2021

This file provides the definitions of the class methods declared in MotorController.h.  Please see that file for more details.

**************************************************************************************************************************************/

#include "MotorController.h"

team967::MotorController::MotorController(int pin, int channel, int frequency, int resolution): p(pin), chan(channel), freq(frequency), res(resolution) {
    // maximum supported resolution is 16 bits
    if(resolution > 16) resolution = 16;

    dutyCycleMin = ((1<<res) / 6);
    dutyCycleMax = ((1<<res) / 3);

    inv = false;
}

team967::MotorController::~MotorController() {
    ledcDetachPin(p);
}

void team967::MotorController::setInverted(bool inverted) {
    inv = inverted;
}

bool team967::MotorController::isInverted() { 
    return inv;
}

void team967::MotorController::setFreq(int f) {
    freq = f;
}

void team967::MotorController::begin() {
    pinMode(p, OUTPUT);

    // initialize the PWM channel used for the motor controller
    // uses LED PWM, but these seem to generate the correct waveforms so it should be fine
    ledcSetup(chan, freq, res);
    ledcAttachPin(p, chan);
}

void team967::MotorController::set(int16_t power) {
   // the neutral duty cycle is 1/4 of the full duty cycle, which is why the - 2 is here
    uint16_t dutyCycle = (1<<(res - 2));

    int16_t divisor = (1<<res) / (dutyCycleMax - dutyCycleMin);

    if(!inv) {
        dutyCycle += power / divisor;
    } else {
        dutyCycle -= power / divisor;
    }

    // I don't think we can exceed these bounds, but check just to be safe
    if(dutyCycle > dutyCycleMax) dutyCycle = dutyCycleMax;
    if(dutyCycle < dutyCycleMin) dutyCycle = dutyCycleMin;

    ledcWrite(chan, dutyCycle);
}