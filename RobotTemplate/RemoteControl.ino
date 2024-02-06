/*
  RemoteControl.ino - Arduino Sketch for Remote Control of the RLSK robot
  
  Description:
  This sketch provides functions for controlling an RLSK robot using a
  playstation controller for input. A few actions are programed for an 
  example. 

  Button map:
  Square button puts the robot in MANUAL
  Circle button puts the robot in AUTONOMOUS
  R2 engages the claw
  R1 releases the claw
  Right Stick controls the movement in MANUAL
  Cross button calibrates the line sensor
  L2 transmits the IR light
  
  Created by: Underachieving Unicorns
  Date: 2/5/2024
  Version: 1.0
*/

void RemoteControl() {
  ps2x.read_gamepad();
  turnValue = -(ps2x.Analog(PSS_RX) - 127);
  rightStick = -(ps2x.Analog(PSS_RY) - 127);


  if (ps2x.Button(PSB_CROSS) && !calibrated) {
    RobotCurrentState = CALIBRATE;
    calibrated = true;
  }

  if (ps2x.Button(PSB_L2)) {  //IR transmit
    IRTransmit(0xEE, 0xEE);
  }

  if (ps2x.Button(PSB_R2)) {  // Activate Claw
    currentMillis = millis();
    clawOn = true;
  }
  if (clawOn) {  // Claw controls
    if (clawPos < 140) {
      clawPos = 40 + (millis() - currentMillis) / 10;
      myservo.write(clawPos);
    }
    if (ps2x.Button(PSB_R1)) {
      clawOn = false;
      clawPos = 40;
      myservo.write(clawPos);
    }
  }

  if (rightStick > 10 || rightStick < -10 || turnValue > 10 || turnValue < -10) {  //Movement Code for right stick input
    enableMotor(2);
    leftMotorSpeed = map((map(rightStick, 0, 128, 0, 100) - map(turnValue, 0, 128, 0, 50)), -150, 150, -100, 100) / 2;
    rightMotorSpeed = map((map(rightStick, 0, 128, 0, 100) + map(turnValue, -128, 0, -50, 0)), -150, 150, -100, 100) / 2;
    if (leftMotorSpeed < 0) setMotorDirection(0, 1);
    else if (leftMotorSpeed > 0) setMotorDirection(0, 0);
    if (rightMotorSpeed < 0) setMotorDirection(1, 1);
    else if (rightMotorSpeed > 0) setMotorDirection(1, 0);
    Serial.print("Left motor speed: ");
    Serial.println(leftMotorSpeed);
    Serial.print("Right motor speed: ");
    Serial.println(rightMotorSpeed);
    setMotorSpeed(0, abs(leftMotorSpeed));
    setMotorSpeed(1, abs(rightMotorSpeed));
  } else setMotorSpeed(2, 0);
}
