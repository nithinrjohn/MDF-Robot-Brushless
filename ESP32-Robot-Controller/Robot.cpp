#include "Robot.h"

team967::Robot::Robot(GameController &gc)
{
    gameController = gc;
}

void team967::Robot::begin()
{
    Serial.begin(115200);
    pinMode(PCB_GPIO_6, INPUT);
    pinMode(ESP32_ONBOARD_LED, OUTPUT);

    gameController.begin();

    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    //set motors
    for(int i = 0; i < 5; i++) {
        motors[i].setMotor();
    }

    delay(1000);
    
    // write a throttle signal to trigger a low beep
    for(int i = 0; i < 5; i++) {
        motors[i].servo.write(1550);
    }

    delay(1000);

    // write a zero throttle to end arming sequence and high beep
    for(int i = 0; i < 5; i++) {
        motors[i].servo.write(1000);
    }

    delay(1000);

    for(int i = 0; i < 5; i++) {
        motors[i].servo.write(1400);
    }

    delay(500);

    for(int i = 0; i < 5; i++) {
        motors[i].servo.write(1500);
    }

    delay(500);

    while(!gameController.isConnected()) {
        Serial.println("Waiting for controller connection...");
        delay(3000);
    }

    Serial.println("Successfully connected to game controller");

}

void team967::Robot::blueLight(bool state)
{
    if(state)
    {
        digitalWrite(ESP32_ONBOARD_LED, HIGH);
    }
    else if(!state)
    {
        digitalWrite(ESP32_ONBOARD_LED, LOW);
    }
}

void team967::Robot::runAuto(void (*autoFunc)())
{
    if(gameController.isButtonPressed(CROSS) && firstTimeAuto)
    {
        firstTimeAuto = false;
        autoFunc();
    }
}

void team967::Robot::drive()
{
    if(!firstTimeAuto)
    {
        short left = -gameController.getLeftStickY() + gameController.getRightStickX();
        short right = -gameController.getLeftStickY() - gameController.getRightStickX();

        left = (left > 127) ? 127 : left;
        left = (left < -128) ? -128 : left;
        right = (right > 127) ? 127 : right;
        right = (right < -128) ? -128 : right;

        motors[LEFT].servo.write(1500 + left*3);
        motors[RIGHT].servo.write(1500 + right*3);
    }
}

void team967::Robot::stopMotors()
{
    for(int i = 0; i < 5; i++) {
        motors[i].setPower(0); 
    }
}

void team967::Robot::rollers()
{
    if(!firstTimeAuto)
    {
        if(gameController.isButtonPressed(LEFT_TRIGGER)) {
            motors[FRONT_ROLLER].setPower(1);
            motors[MID_ROLLER].setPower(1);
        }
        else if(gameController.isButtonPressed(RIGHT_BUMP))
        {
            motors[MID_ROLLER].setPower(1);
        }
        else if(gameController.isButtonPressed(RIGHT_TRIGGER))
        {
            motors[FLYWHEEL].setPower(1);
            // if(shoot_time == 0)
            // {
            //     shoot_time = esp_timer_get_time();
            // }
            // if((esp_timer_get_time() - shoot_time) > (powerUpTime * 1000000))
            // {
            //     motors[MID_ROLLER].setPower(1);
            // }
        }
        else
        {
            //shoot_time = 0;
            motors[FRONT_ROLLER].setPower(0);
            motors[MID_ROLLER].setPower(0);
            motors[FLYWHEEL].setPower(0);
        }
    }
}

void team967::Robot::shoot(float power)
{
    motors[FLYWHEEL].setPower(power);
    delay(2000);
    motors[MID_ROLLER].setPower(1);
    delay(1000);
    motors[MID_ROLLER].setPower(0);
    motors[FLYWHEEL].setPower(0);
}

void team967::Robot::intake(int seconds)
{
    motors[FRONT_ROLLER].setPower(1);
    motors[MID_ROLLER].setPower(1);
    delay(seconds * 1000);
    motors[FRONT_ROLLER].setPower(0);
    motors[MID_ROLLER].setPower(0);
}

void team967::Robot::move(float power, int seconds)
{
    motors[RIGHT].setPower(power);
    motors[LEFT].setPower(power);
    delay(seconds * 1000);
    motors[RIGHT].setPower(0);
    motors[LEFT].setPower(0);
}

void team967::Robot::turn(float power, int seconds, int wheel)
{
    if(wheel == BOTH)
    {
        motors[RIGHT].setPower(-power);
        motors[LEFT].setPower(power);
        delay(seconds * 1000);
        motors[RIGHT].setPower(0);
        motors[LEFT].setPower(0);
    }
    else if(wheel == RIGHT)
    {
        motors[RIGHT].setPower(power);
        motors[LEFT].setPower(0);
        delay(seconds * 1000);
        motors[RIGHT].setPower(0);
        motors[LEFT].setPower(0);
    }
    else if(wheel == LEFT)
    {
        motors[RIGHT].setPower(0);
        motors[LEFT].setPower(power);
        delay(seconds * 1000);
        motors[RIGHT].setPower(0);
        motors[LEFT].setPower(0);
    }
}