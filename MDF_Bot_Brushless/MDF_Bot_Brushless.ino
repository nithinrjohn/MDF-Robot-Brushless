#include <GameController.h>
#include <Robot.h>

using namespace team967; 

GameController gameController("00:00:00:00:00:00");  

Robot robot(gameController);

void setup() {  
  robot.begin();
}  

void autonomous() {
  Serial.println("ran auto");
  //auto code here
}

void loop() {
  bool ranAuto = robot.runAuto(&autonomous);

  if(gameController.isConnected()) {
    digitalWrite(ESP32_ONBOARD_LED, HIGH);
  }
  else {    
    digitalWrite(ESP32_ONBOARD_LED, LOW);
  }
  if(gameController.isConnected() && ranAuto) {

    robot.tankDrive();
    robot.rollers();
  }
}