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
  robot.runAuto(&autonomous);

  if(gameController.isConnected()) {
    robot.blueLight(ON);
    robot.drive();
    robot.rollers();
  }
  else {    
    robot.blueLight(OFF);
  }
}