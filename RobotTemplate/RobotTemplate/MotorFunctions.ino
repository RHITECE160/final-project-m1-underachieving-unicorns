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
void followLine() {
  uint16_t totalCount = 0;
  uint16_t leftCount, rightCount;
  uint16_t target = countForDistance(wheelDiameter, cntPerRevolution, inchesToTravelLineFollow);
  uint32_t linePos = getLinePosition();

  resetLeftEncoderCnt();
  resetRightEncoderCnt();
  setMotorDirection(BOTH_MOTORS, MOTOR_DIR_FORWARD);
  enableMotor(BOTH_MOTORS);
  if ((linePos > 0) && (linePos < 4000)) {  // turn left
    setMotorSpeed(LEFT_MOTOR, 10);
    setMotorSpeed(RIGHT_MOTOR, 20);
  } else if (linePos > 5000) {  // turn right
    setMotorSpeed(LEFT_MOTOR, 20);
    setMotorSpeed(RIGHT_MOTOR, 10);
  } else {  // go straight
    setMotorSpeed(LEFT_MOTOR, 10);
    setMotorSpeed(RIGHT_MOTOR, 10);
  }
  while (totalCount < target) {
    leftCount = getEncoderLeftCnt();
    rightCount = getEncoderRightCnt();
    totalCount = (leftCount + rightCount) / 2;
    UART_SERIAL.print("\t");
    UART_SERIAL.print(leftCount);
    UART_SERIAL.print("\t");
    UART_SERIAL.println(rightCount);
  }
  disableMotor(BOTH_MOTORS);
  float traveled = distanceTraveled(wheelDiameter, cntPerRevolution, totalCount);
  UART_SERIAL.print("Distance traveled (inches): ");
  UART_SERIAL.println(traveled);
  UART_SERIAL.println();
}

void forward(int x) {
  int start = millis();
  while (millis() < start + x) {
    enableMotor(BOTH_MOTORS);
    setMotorDirection(LEFT_MOTOR, MOTOR_DIR_FORWARD);
    setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_FORWARD);
    setMotorSpeed(BOTH_MOTORS, fastSpeed + 10);
  }
}
void backwards(int x) {
  int start = millis();
  while (millis() < start + x) {
    enableMotor(BOTH_MOTORS);
    setMotorDirection(LEFT_MOTOR, MOTOR_DIR_BACKWARD);
    setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_BACKWARD);
    setMotorSpeed(BOTH_MOTORS, fastSpeed);
  }
}
void turnRight(int x) {
  int start = millis();
  while (millis() < start + x) {
    enableMotor(BOTH_MOTORS);
    setMotorDirection(LEFT_MOTOR, MOTOR_DIR_FORWARD);
    setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_BACKWARD);
    setMotorSpeed(BOTH_MOTORS, 25);
  }
}

float distanceTraveled(float wheel_diam, uint16_t cnt_per_rev, uint8_t current_cnt) {
  float temp = (wheel_diam * PI * current_cnt) / cnt_per_rev;
  return temp;
}

uint32_t countForDistance(float wheel_diam, uint16_t cnt_per_rev, uint32_t distance) {
  float temp = (wheel_diam * PI) / cnt_per_rev;
  temp = distance / temp;
  return int(temp);
}

/* Stops robot forward: both motors disabled */
void stop() {
  disableMotor(BOTH_MOTORS);
}