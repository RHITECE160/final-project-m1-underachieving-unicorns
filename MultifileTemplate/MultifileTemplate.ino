/*A multifile project code template
  A template for the Milestone 1 project code that uses multiple files
  for modularity. The compiler first loads the principal file 
  (the one with the same name as the folder) and then loads 
  the others in alphabetical order. Variables defined in an 
  earlier file will be visible to code in a file that is loaded 
  later, but not vice-versa. 

  Calls functions in files:
  MotorFunctions.ino

  written for the MSP432401 board
  Author: Deborah Walter
  Last revised: 11/28/2023

***** Hardware Connections: *****
     start button       P3.0
     playstation connections
     brown wire         P1.7 
     orange wire        P1.6 
     yellow wire        P2.3
     blue wire          P6.7
*/

// Load libraries used
#include "SimpleRSLK.h"
#include <Servo.h>
#include "PS2X_lib.h"

// Define pin numbers for the button on the PlayStation controller
#define PS2_DAT 26  //P1.7 <-> brown wire
#define PS2_CMD 5   //P1.6 <-> orange wire
#define PS2_SEL 37  //P2.3 <-> yellow wire (also called attention)
#define PS2_CLK 2   //P6.7 <-> blue wire

// Create an instance of the playstation controller object
PS2X ps2x;

// Define remote mode either playstation controller or IR remote controller
enum RemoteMode {
  PLAYSTATION,
  IR_REMOTE,
};

// Declare and initialize the current state variable
RemoteMode CurrentRemoteMode = PLAYSTATION;

// Tuning Parameters
const uint16_t lowSpeed = 15;
const uint16_t fastSpeed = 30;

int turnValue;
int rightStick;
int leftMotorSpeed;
int rightMotorSpeed;
int acceleration;
int clawPos;
int currentMillis;

boolean clawOn = false;
Servo myservo;

void setup() {
  Serial.begin(57600);
  Serial.print("Starting up Robot code...... ");
  myservo.attach(SRV_0);
  // Run setup code
  setupRSLK();

  if (CurrentRemoteMode == 0) {
    // using the playstation controller
    Serial.println("Using playstation controller, make sure it is paired first ");

    // Initialize PlayStation controller
    delayMicroseconds(500 * 1000);  //added delay to give wireless ps2 module some time to startup, before configuring it
    // declare variables for playstation control
    bool pressures = false;
    bool rumble = false;
    int error = 1;

    while (error) {
      error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

      if (error == 0)
        Serial.println("Found Controller, configured successful ");

      else if (error == 1)
        Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

      else if (error == 2)
        Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

      else if (error == 3)
        Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
      delayMicroseconds(1000 * 1000);
    }
  } else if (CurrentRemoteMode == 1) {
    // put start-up code for IR controller here if neccessary
  }
}

void loop() {
  // Read input from PlayStation controller
  ps2x.read_gamepad();

  // Operate the robot in remote control mode
  if (CurrentRemoteMode == 0) {
    RemoteControlPlaystation();

  } else if (CurrentRemoteMode == 1) {
    // put code here to run using the IR controller if neccessary
  }
}


/* RemoteControlPlaystation() function
  This function uses a playstation controller and the PLSK libraray with
  an RLSK robot using to implement remote controller. 
  
  A few actions are programed for an example. 

  Button control map:
  PAD UP button moves both motors forward
  CROSS button stops motors
  */
void RemoteControlPlaystation() {
  while (CurrentRemoteMode == 0) {
    ps2x.read_gamepad();
    turnValue = -(ps2x.Analog(PSS_RX) - 127);
    rightStick = -(ps2x.Analog(PSS_RY) - 127);

    if (ps2x.Button(PSB_R2)) {
      currentMillis = millis();
      clawOn = true;
    }
    if (clawOn) {
      if (clawPos < 140) {
        clawPos = 40 + (millis() - currentMillis) / 10;
        Serial.println(clawPos);
        myservo.write(clawPos);
      }
      if (ps2x.Button(PSB_R1)) {
        Serial.println("R1 ON");
        clawOn = false;
        clawPos = 40;
        myservo.write(clawPos);
        Serial.println(clawPos);
      }
    }

    if (rightStick > 10 || rightStick < -10 || turnValue > 10 || turnValue < -10) {
      enableMotor(2);
      leftMotorSpeed = map((map(rightStick, 0, 128, 0, 100) - map(turnValue, 0, 128, 0, 50)), -150, 150, -100, 100);
      rightMotorSpeed = map((map(rightStick, 0, 128, 0, 100) + map(turnValue, -128, 0, -50, 0)), -150, 150, -100, 100);
      if (leftMotorSpeed < 0) setMotorDirection(0, 1);
      else if (leftMotorSpeed > 0) setMotorDirection(0, 0);
      if (rightMotorSpeed < 0) setMotorDirection(1, 1);
      else if (rightMotorSpeed > 0) setMotorDirection(1, 0);
      setMotorSpeed(0, abs(leftMotorSpeed));
      setMotorSpeed(1, abs(rightMotorSpeed));
    } else setMotorSpeed(2, 0);
  }
  // put your code here to run in remote control mode

  // Example of receive and decode remote control command
  // the forward() and stop() functions should be independent of
}
