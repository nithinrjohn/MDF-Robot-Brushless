#pragma once

#include <ESP32Servo.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>
#include <GameController.h>

namespace team967 {
    class Motor {
        private:
            int id;
            int pin;
        public:
            Servo servo;
            Motor();
            Motor(int id, int pin);
            void setMotor();
            void setPower(float power);
    };
}
    