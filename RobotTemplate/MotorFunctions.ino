/*
  MotorFunctions.ino - Arduino Sketch for Motor Control
  
  Description:
  This sketch provides functions for controlling a RLSK robot, allowing the
  motors to move forward and stop.

  Functions:
  1. void moveForward()
     - Activates the motor to move forward.

  2. void stopMotor()
     - Stops the motor.

  Created by: Your Name
  Date: Current Date
  Version: 1.0
*/

/* Moves robot forward: both motors forward same speed */
void forward() {
  
  uint32_t linePos = getLinePosition();

  if ((linePos > 0) && (linePos < 4000)) {  // turn left
    setMotorSpeed(LEFT_MOTOR, normalSpeed);
    setMotorSpeed(RIGHT_MOTOR, fastSpeed);
  } else if (linePos > 5000) {  // turn right
    setMotorSpeed(LEFT_MOTOR, fastSpeed);
    setMotorSpeed(RIGHT_MOTOR, normalSpeed);
  } else {  // go straight
    setMotorSpeed(LEFT_MOTOR, normalSpeed);
    setMotorSpeed(RIGHT_MOTOR, normalSpeed);
  }
}

/* Stops robot forward: both motors disabled */
void stop() {
  disableMotor(BOTH_MOTORS);
}