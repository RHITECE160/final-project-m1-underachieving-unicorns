/*
  AutonomousControl.ino - Arduino Sketch for Autonomous Control of the RLSK robot
  
  Description:
  This sketch provides functions for autonomous action of an RLSK robot. A state
  machine is used for autonomous control. A few actions have been programed as
  an example. 

  The robot moves forward for a time (defined by movementDuration). Then 
  transitions to a number of other states. After reaching the final state, 
  the higher-level state is switched to IDLE to await the next instruction.

  State Machine description:
  The robot follows the line for a specified distance, then moves forward until it hits a wall. 
  Then, it moves backwards, turns to the right, moves forward for a specified amount of time, 
  then follows the line and moves to the end. Once it reaches the end, it moves backwards a short
  distance then releases the marigold.


  Created by: Underachieving Unicorns
  Date: 2/4/24
  Version: 1.0
*/
void AutonomousControl() {
  while (AutoCurrentState != IDLE) {
    switch (AutoCurrentState) {
      case START: //Start State
        Serial.println("in Autonomous mode the current state: START");
        AutoCurrentState = AUTO_ACTION1;  // Transition to next state
        break;

      case AUTO_ACTION1: //First action, follows line
        Serial.println("in Autonomous mode the current state: AUTO_ACTION1");
        followLine();
        AutoCurrentState = AUTO_ACTION2;
        break;

      case AUTO_ACTION2: //Second action, moves forward until bump switch, then moves backwards
        Serial.println("in Autonomous mode the current state: AUTO_ACTION2");
        if (getBumpSwitchPressed() > 0) {
          stop();  //stop the forward movement
          Serial.println("backwards");
          backwards();
          // Placeholder delay
          AutoCurrentState = AUTO_ACTION3;  // Transition to next state
        } else forward(50);
        break;

      case AUTO_ACTION3: //Third action, turns to the right and moves forward
        Serial.println("in Autonomous mode the current state: AUTO_ACTION3");
        // // Add state instructions here
        Serial.println("right");
        turnRight();  // Placeholder delay
        Serial.println("forward");
        forward(3000);
        AutoCurrentState = AUTO_ACTION4;  // Transition to next state
        break;

      case AUTO_ACTION4: //Fourth action, line follows then moves until it hits the end then goes backwards and drops the marigold
        Serial.println("in Autonomous mode the current state: AUTO_ACTION4");
         followLine();
        while (!switchPressed) {
          Serial.println("in Autonomous mode the current state: AUTO_ACTION4");
          forward(250);
          if (getBumpSwitchPressed() > 0) {
            switchPressed = true;
          }
        }
        backwards();
        clawPos = 40;
        myservo.write(clawPos);
        AutoCurrentState = IDLE;
        clawOn = false;
        break;

      default:
        // Handle unknown state, if needed
        break;
    }
  }

  // The code will exit the while loop when IDLE state is reached
  Serial.println("State: IDLE");
  RobotCurrentState = MANUAL;
}