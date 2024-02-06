/*
  MotorFunctions.ino - Arduino Sketch for Motor Control
  
  Description:
  This sketch provides functions for controlling a RLSK robot, allowing the
  motors to move forward and stop.

  Functions:
  1. void followLine()
     - Activates the motor to move forward, following the line.

  2. void forward(int x)
     - Activates the motor to move forward for an input millisecond duration.

  3. void backwards()
     - Activates the motor to move backwards a specified amount of inches.

  4. void turnRight()
     - Activates the motor to turn the robot right 90 degrees.

  5. void floorCalibration()
     - Calibrates the line sensor for line following.

  6. float distanceTraveled(float wheel_diam, uint16_t cnt_per_rev, uint8_t current_cnt)
     - Uses the encoders to determine distance traveled for debugging purposes.

  7. uint32_t countForDistance(float wheel_diam, uint16_t cnt_per_rev, uint32_t distance)
     - Calculates the distance the robot will need to travel.

  8. void stop()
     - Stops the robot's movement.
  Created by: Underachieving Unicorns
  Date: 2/5/2024
  Version: 1.0
*/

/* Moves robot forward: both motors forward same speed */
void followLine() {
  uint16_t totalCount = 0;
  uint16_t leftCount, rightCount;
  uint16_t target = countForDistance(wheelDiameter, cntPerRevolution, inchesToTravelLineFollow);

  resetLeftEncoderCnt();
  resetRightEncoderCnt();
  setMotorDirection(BOTH_MOTORS, MOTOR_DIR_FORWARD);
  enableMotor(BOTH_MOTORS);

  while (totalCount < target) {
    uint32_t linePos = getLinePosition();
    if ((linePos > 0) && (linePos < 4000)) {  // turn left
      setMotorSpeed(LEFT_MOTOR, normalSpeed);
      setMotorSpeed(RIGHT_MOTOR, fastSpeed);
    } else if (linePos > 5000) {  // turn right
      setMotorSpeed(LEFT_MOTOR, fastSpeed);
      setMotorSpeed(RIGHT_MOTOR, normalSpeed);
    } else {  // go straight
      setMotorSpeed(LEFT_MOTOR, fastSpeed);
      setMotorSpeed(RIGHT_MOTOR, fastSpeed);
    }
    leftCount = getEncoderLeftCnt();
    rightCount = getEncoderRightCnt();
    totalCount = (leftCount + rightCount) / 2;
    // UART_SERIAL.print("\t");
    // UART_SERIAL.print(leftCount);
    // UART_SERIAL.print("\t");
    // UART_SERIAL.println(rightCount);
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
void backwards() {
  uint16_t totalCount = 0;
  uint16_t leftCount, rightCount;
  uint16_t target = countForDistance(wheelDiameter, cntPerRevolution, inchesToTravelBack);
  

  /* Set the encoder pulses count back to zero */
  resetLeftEncoderCnt();
  resetRightEncoderCnt();

  /* Cause the robot to drive forward */
  setMotorDirection(BOTH_MOTORS, MOTOR_DIR_BACKWARD);

  /* "Turn on" the motor */
  enableMotor(BOTH_MOTORS);

  /* Set motor speed */
  setMotorSpeed(BOTH_MOTORS, fastSpeed);

  /* Drive motor until it has received x pulses */
  while (totalCount < target) {
    leftCount = getEncoderLeftCnt();
    rightCount = getEncoderRightCnt();
    totalCount = (leftCount + rightCount) / 2;
    UART_SERIAL.print("\t");
    UART_SERIAL.print(leftCount);
    UART_SERIAL.print("\t");
    UART_SERIAL.println(rightCount);
  }

  /* Halt motors */
  disableMotor(BOTH_MOTORS);

  /* Calculate traveled distance from encoder ticks */
  float traveled = distanceTraveled(wheelDiameter, cntPerRevolution, totalCount);
  UART_SERIAL.print("Distance traveled (inches): ");
  UART_SERIAL.println(traveled);
  UART_SERIAL.println();
}
void turnRight() {
   uint16_t totalCount = 0;
  uint16_t leftCount, rightCount;
  uint16_t target = countForDistance(wheelDiameter, cntPerRevolution, inchesToTurnRight);
  

  /* Set the encoder pulses count back to zero */
  resetLeftEncoderCnt();
  resetRightEncoderCnt();

  /* Cause the robot to drive forward */
  setMotorDirection(LEFT_MOTOR, MOTOR_DIR_FORWARD);
  setMotorDirection(RIGHT_MOTOR, MOTOR_DIR_BACKWARD);

  /* "Turn on" the motor */
  enableMotor(BOTH_MOTORS);

  /* Set motor speed */
  setMotorSpeed(BOTH_MOTORS, normalSpeed);

  /* Drive motor until it has received x pulses */
  while (totalCount < target) {
    leftCount = getEncoderLeftCnt();
    rightCount = getEncoderRightCnt();
    totalCount = (leftCount + rightCount) / 2;
    UART_SERIAL.print("\t");
    UART_SERIAL.print(leftCount);
    UART_SERIAL.print("\t");
    UART_SERIAL.println(rightCount);
  }

  /* Halt motors */
  disableMotor(BOTH_MOTORS);

  /* Calculate traveled distance from encoder ticks */
  float traveled = distanceTraveled(wheelDiameter, cntPerRevolution, totalCount);
  UART_SERIAL.print("Distance traveled (inches): ");
  UART_SERIAL.println(traveled);
  UART_SERIAL.println();
}


void floorCalibration() {
  /* Place Robot On Floor (no line) */

  UART_SERIAL.println("Running calibration on floor");

  /* Set both motors direction forward */
  setMotorDirection(BOTH_MOTORS, MOTOR_DIR_FORWARD);
  /* Enable both motors */
  enableMotor(BOTH_MOTORS);
  /* Set both motors speed 20 */
  setMotorSpeed(BOTH_MOTORS, 20);

  /* Must be called prior to using getLinePosition() or readCalLineSensor() */
  calibrateLineSensor(lineColor);

  /* Disable both motors */
  disableMotor(BOTH_MOTORS);

  UART_SERIAL.println("Reading floor values complete");

  UART_SERIAL.println("Push left button on Launchpad to begin line following.");
  UART_SERIAL.println("Make sure the robot is on the line.\n");
  /* Wait until button is pressed to start robot */


  enableMotor(BOTH_MOTORS);
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