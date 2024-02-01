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
/* Diameter of Romi wheels in inches 
 *  2.7559055" = 7.0 cm (International System of Units) 
 */
const float wheelDiameter = 2.7559055; 

/* Number of encoder (rising) pulses every time the wheel turns completely */
const int cntPerRevolution = 360;


const int wheelSpeed = 15; // Default raw pwm speed for motor.
/* Moves robot forward: both motors forward same speed */
void followLine() {

  uint32_t linePos = getLinePosition();
  if (ps2x.Button(PSB_SQUARE)) {
    Serial.print("start button has been pressed going to manual");
    //go to Manual state when start button pushed
    RobotCurrentState = MANUAL;
  }
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

void forward() {
  enableMotor(BOTH_MOTORS);
  setMotorDirection(LEFT_MOTOR, MOTOR_DIR_FORWARD);
  setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_FORWARD);
  setMotorSpeed(BOTH_MOTORS, fastSpeed);
}
void backwardsEncoder(int degrees){
  resetLeftEncoderCnt();
  resetRightEncoderCnt();
  enableMotor(BOTH_MOTORS);
  setMotorDirection(BOTH_MOTORS, MOTOR_DIR_BACKWARD);
  setMotorSpeed(BOTH_MOTORS, 15);
  while((getEncoderRightCnt()+getEncoderLeftCnt())/2 < degrees){
  }
  disableMotor(BOTH_MOTORS);
}
void turnRight(int degrees) {
  resetLeftEncoderCnt();
  resetRightEncoderCnt();
  enableMotor(BOTH_MOTORS);
  setMotorDirection(LEFT_MOTOR, MOTOR_DIR_FORWARD);
  setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_BACKWARD);
  setMotorSpeed(BOTH_MOTORS, 15);
  while((getEncoderRightCnt()<degrees)){
  }
  disableMotor(BOTH_MOTORS);
}
/* Stops robot forward: both motors disabled */
void stop() {
  disableMotor(BOTH_MOTORS);
}