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

bool team967::Robot::runAuto(void (*autoFunc)())
{
    if(gameController.isButtonPressed(TRIANGLE) && firstTimeAuto)
    {
        firstTimeAuto = false;
        autoFunc();
    }
    else if(gameController.isButtonPressed(CROSS) && firstTimeAuto)
    {
        firstTimeAuto = false;
    }
    return !firstTimeAuto;
}

void team967::Robot::arcadeDrive()
{
    if(gameController.isButtonPressed(CIRCLE))
    {
        stopMotors();
    }
    else
    {
        short left = gameController.getLeftStickY() + gameController.getRightStickX();
        short right = -gameController.getLeftStickY() - gameController.getRightStickX();

        left = (left > 127) ? 127 : left;
        left = (left < -128) ? -128 : left;
        right = (right > 127) ? 127 : right;
        right = (right < -128) ? -128 : right;

        float clamp = 3.90625;

        motors[LEFT].servo.write(1500 + left*clamp);
        motors[RIGHT].servo.write(1500 + right*clamp);
    }
}

void team967::Robot::tankDrive()
{
    if(gameController.isButtonPressed(CIRCLE))
    {
        stopMotors();
    }
    else
    {
        short left = gameController.getLeftStickY();
        short right = gameController.getRightStickY();

        left = (left > 127) ? 127 : left;
        left = (left < -128) ? -128 : left;
        right = (right > 127) ? 127 : right;
        right = (right < -128) ? -128 : right;

        float clamp = 3.90625;

        motors[LEFT].servo.write(1500 + left*clamp);
        motors[RIGHT].servo.write(1500 + right*clamp);
    }
}

void team967::Robot::dPadDrive()
{
    if(gameController.isButtonPressed(CIRCLE))
    {
        stopMotors();
    }
    else if(gameController.isButtonPressed(D_UP))
    {
        motors[LEFT].setPower(1);
        motors[RIGHT].setPower(1);
    }
    else if(gameController.isButtonPressed(D_DOWN))
    {
        motors[LEFT].setPower(-1);
        motors[RIGHT].setPower(-1);
    }
    else if(gameController.isButtonPressed(D_RIGHT))
    {
        motors[LEFT].setPower(0);
        motors[RIGHT].setPower(1);
    }
    else if(gameController.isButtonPressed(D_LEFT))
    {
        motors[LEFT].setPower(1);
        motors[RIGHT].setPower(0);
    }
    else
    {
        motors[LEFT].setPower(0);
        motors[RIGHT].setPower(0);
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
    if(gameController.isButtonPressed(LEFT_TRIGGER)) {
        motors[FRONT_ROLLER].setPower(1);
        motors[MID_ROLLER].setPower(1);
    }
    else if(gameController.isButtonPressed(RIGHT_TRIGGER))
    {
        power = 1;
        motors[FLYWHEEL].setPower(power);
        shot = true;
    }
    else if(gameController.isButtonPressed(RIGHT_BUMP))
    {
        motors[MID_ROLLER].setPower(1);
    }
    else
    {
        if(shot)
        {
            power -= 0.0001;
            motors[FLYWHEEL].setPower(power);
            if(power <= 0)
            {
                shot = false;
            }
        }
        motors[FRONT_ROLLER].setPower(0);
        motors[MID_ROLLER].setPower(0);
    }
}

//auto

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