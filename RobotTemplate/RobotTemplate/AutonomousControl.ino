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
  The robot moves forward for a short time using a non-blocking delay, then 
  the motors stop. Then the next few states are place-holders. The Robot returns
  to IDLE state when autonomous functions are complete.

  Created by: Your Name
  Date: Current Date
  Version: 1.0
*/
void AutonomousControl() {
  // put your code here to run in Autonomous control mode

  unsigned long myTime;

  Serial.println("in the AutonomousControl function");

  // State machine loop
  while (AutoCurrentState != IDLE) {
    switch (AutoCurrentState) {
      case START:
        Serial.println("in Autonomous mode the current state: START");
        // Add START state instructions here
        AutoCurrentState = AUTO_ACTION1;  // Transition to next state
        lastActionTime = millis();        // Record the time when the forward state started
        break;

      case AUTO_ACTION1:
        Serial.println("in Autonomous mode the current state: AUTO_ACTION1");
        //move forward for a time, then stop, and transition to the next state
        /* Set both motors direction forward */
        setMotorDirection(BOTH_MOTORS, MOTOR_DIR_FORWARD);
        /* Enable both motors */
        enableMotor(BOTH_MOTORS);
        /* Set both motors speed 20 */
        setMotorSpeed(BOTH_MOTORS, 20);

        /* Must be called prior to using getLinePosition() or readCalLineSensor() */
        calibrateLineSensor(lineColor);
        disableMotor(BOTH_MOTORS);
        delay(500);
        enableMotor(BOTH_MOTORS);
        while (millis() - lastActionTime < movementDuration) {
          followLine();
        }
        // Check if the movement duration has passed
        if (millis() - lastActionTime >= movementDuration) {
          stop();                           //stop the forward movement
          AutoCurrentState = AUTO_ACTION2;  // Transition to next state
          lastActionTime = millis();        // Record the time when the next state started
        }


        break;

      case AUTO_ACTION2:
        Serial.println("in Autonomous mode the current state: AUTO_ACTION2");
        

        if (getBumpSwitchPressed() > 0) {
          stop();  //stop the forward movement
          Serial.println("backwards");
          backwards(825);                   // Placeholder delay
          AutoCurrentState = AUTO_ACTION3;  // Transition to next state
        } else forward(50);
        break;

      case AUTO_ACTION3:
        Serial.println("in Autonomous mode the current state: AUTO_ACTION3");
        // Add state instructions here
        Serial.println("right");
        turnRight(330);  // Placeholder delay
        Serial.println("forward");
        forward(2000);
        AutoCurrentState = AUTO_ACTION4;  // Transition to next state
        break;

      case AUTO_ACTION4:
      lastActionTime = millis();  
      while (millis() - lastActionTime < movementDuration) {
          followLine();
        }
        // Check if the movement duration has passed
        if (millis() - lastActionTime >= movementDuration) {
          stop();                           //stop the forward movement
          lastActionTime = millis();        // Record the time when the next state started
        }
        while (!switchPressed) {
          Serial.println("in Autonomous mode the current state: AUTO_ACTION4");
          forward(250);
          if (getBumpSwitchPressed() > 0) {
            switchPressed = true;
          }
        }
        backwards(250);
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
  // Add IDLE state instructions here
  
        RobotCurrentState = MANUAL;

}