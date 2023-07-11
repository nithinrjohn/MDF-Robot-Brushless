#pragma once

#include <CustomBoardPinDefs.h>
#include <ESP32Servo.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>
#include <GameController.h>
#include <Motor.h>

namespace team967 {

    const bool ON = true;
    const bool OFF = false;

    const int LEFT = 0;
    const int RIGHT = 1;
    const int BOTH = 2;
    const int FRONT_ROLLER = 2;
    const int MID_ROLLER = 3;
    const int FLYWHEEL = 4;
    //must be between 1 and 0
    const float driveSpeedMultiplier = 1;

    class Robot {
        private:
            uint64_t shoot_time = 0;
            int powerUpTime = 2;
            GameController gameController;
            bool firstTimeAuto = true;
        public:
            Motor motors[5] {
                Motor(LEFT, PCB_GPIO_0),
                Motor(RIGHT, PCB_GPIO_1),
                Motor(FRONT_ROLLER, PCB_GPIO_2),
                Motor(MID_ROLLER, PCB_GPIO_3),
                Motor(FLYWHEEL, PCB_GPIO_4)
            };
            Robot(GameController &gc);
            void begin();
            void blueLight(bool state);
            void runAuto(void (*autoFunc)());
            void drive();
            void stopMotors();
            void rollers();
            void shoot(float power);
            void intake(int seconds);
            void move(float power, int seconds);
            void turn(float power, int seconds, int wheel);
    };

}
