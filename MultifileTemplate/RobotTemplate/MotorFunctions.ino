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

  Created by: Mateo Olson, Connor Bodie, John Webster 
  Date: 1/22/24 
  Version: 1.0
*/

/* Moves robot forward: both motors forward same speed */
void forward() {
    enableMotor(BOTH_MOTORS);
    setMotorDirection(LEFT_MOTOR, MOTOR_DIR_FORWARD);
    setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_FORWARD);
    setMotorSpeed(BOTH_MOTORS, fastSpeed);
}

void turnLeft(){ //Turn left by turning left motor forward and right motor backwards 
  enableMotor(BOTH_MOTORS);
  setMotorDirection(LEFT_MOTOR, MOTOR_DIR_FORWARD);
  setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_BACKWARD);
  setMotorSpeed(BOTH_MOTORS, fastSpeed);
}

void turnRight(){ //Inverse of turnLeft()
  enableMotor(BOTH_MOTORS);
  setMotorDirection(LEFT_MOTOR, MOTOR_DIR_BACKWARD);
  setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_FORWARD);
  setMotorSpeed(BOTH_MOTORS, fastSpeed);
}
/* Stops robot forward: both motors disabled */
void backward(){ // Inverse of forward()
  enableMotor(BOTH_MOTORS);
  setMotorDirection(LEFT_MOTOR, MOTOR_DIR_BACKWARD);
  setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_BACKWARD);
  setMotorSpeed(BOTH_MOTORS, fastSpeed);
}
void stop() { //stops motors 
    disableMotor(BOTH_MOTORS);
}
