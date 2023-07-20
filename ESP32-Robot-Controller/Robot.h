#pragma once

#include <CustomBoardPinDefs.h>
#include <ESP32Servo.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>
#include <GameController.h>
#include <Motor.h>

namespace team967 {

    const int LEFT = 0;
    const int RIGHT = 1;
    const int BOTH = 2;
    const int FRONT_ROLLER = 2;
    const int MID_ROLLER = 3;
    const int FLYWHEEL = 4;

    class Robot {
        private:
            GameController gameController;
            bool firstTimeAuto = true;
            bool shot = false;
            float power = 1;
        public:
            Motor motors[5] {
                Motor(LEFT, PCB_GPIO_0),
                Motor(RIGHT, PCB_GPIO_1),
                Motor(FRONT_ROLLER, PCB_GPIO_5),
                Motor(MID_ROLLER, PCB_GPIO_3),
                Motor(FLYWHEEL, PCB_GPIO_4)
            };

            Robot(GameController &gc);

            void begin();
            bool runAuto(void (*autoFunc)());
            void stopMotors();
            void rollers(); 
            void dPadDrive();
            void tankDrive();
            void arcadeDrive();

            //auto
            void shoot(float power);
            void intake(int seconds);
            void move(float power, int seconds);
            void turn(float power, int seconds, int wheel);
    };

}
