/*Final project code template
  A template for the final project code that uses multiple files
  for modularity. The compiler first loads the principal file 
  (the one with the same name as the folder) and then loads 
  the others in alphabetical order. Variables defined in an 
  earlier file will be visible to code in a file that is loaded 
  later, but not vice-versa. 

  Calls functions in files:
  AutonomousControl.ino
  MotorFunctions.ino
  RemoteControl.ino
  IRTransmitter.ino

  written for the MSP432401 board
  Author: Underachieving Unicorns
  Last revised: 2/5/2024
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

// Defining for Encoders
#define UART_SERIAL Serial

// Create an instance of the playstation controller object
PS2X ps2x;

// Define high-level state machine
enum RobotState {
  INITIALIZE,  //Starts in Init
  MANUAL,      // Square to go to Manual
  AUTONOMOUS,  // Circle to go to Autonomous from Manual
  CALIBRATE    // Calibration for line following
};

int turnValue;       // Dynamic turning in manual (X position)
int rightStick;      // Checking Right Stick position for forward/backwards (Y position)
int leftMotorSpeed;  // Separate motor speeds to allow for turning
int rightMotorSpeed;
int clawPos;                    // To tell claw to open or close
int currentMillis;              // Timer placeholder
boolean clawOn = false;         // Boolean to check if claw is closed or open
boolean switchPressed = false;  // Boolean to determine if switch has been pressed
boolean calibrated = false;     // Boolean to determine if line following has been calibrated

// Define lower-level state machine for AUTONOMOUS mode
enum AutoState {
  START,
  AUTO_ACTION1,
  AUTO_ACTION2,
  AUTO_ACTION3,
  AUTO_ACTION4,
  IDLE
};

// Declare and initialize the current state variable
RobotState RobotCurrentState = INITIALIZE;
AutoState AutoCurrentState = START;

// Global Variables
unsigned long lastActionTime = 0;  // Variable to store the last time an action was taken

// Tuning Parameters
const uint16_t normalSpeed = 10;
const uint16_t fastSpeed = 20;
const unsigned long movementDuration = 8000;  // Duration for movement forward autonomously in milliseconds
const uint8_t lineColor = LIGHT_LINE;         // Setting color for line following
const float wheelDiameter = 2.7559055;
const int cntPerRevolution = 360;
const int inchesToTravelLineFollow = 24;  //Distance to line follow
const int inchesToTravelBack = 4.2;       // Distance to move backwards
const int inchesToTurnRight = 4.625;      // Distance to turn right
Servo myservo;                            // Setting up servo
uint16_t sensorVal[LS_NUM_SENSORS];
uint16_t sensorCalVal[LS_NUM_SENSORS];
uint16_t sensorMaxVal[LS_NUM_SENSORS];
uint16_t sensorMinVal[LS_NUM_SENSORS];

void setup() {
  Serial.begin(57600);
  Serial.print("Starting up Robot code...... ");
  myservo.attach(SRV_0);
  // Run setup code
  setupRSLK();

  // Initialize PlayStation controller
  delayMicroseconds(500 * 1000);  //added delay to give wireless ps2 module some time to startup, before configuring it
  // declare variables for playstation control
  bool pressures = false;
  bool rumble = false;
  int error = 1;

  while (error) {
    error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);

    if (error == 0)
      Serial.print("Found Controller, configured successful ");

    else if (error == 1)
      Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

    else if (error == 2)
      Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

    else if (error == 3)
      Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
    delayMicroseconds(1000 * 1000);
  }
}

void loop() {
  // Read input from PlayStation controller
  ps2x.read_gamepad();
  delayMicroseconds(50 * 1000);
  // Update state machine based on button input
  updateStateMachine();

  // Perform actions based on the current state
  executeStateActions();
}

/* updateStateMachine function
  This function changes the high-level state based on user input. 
  
  For an example, a SPST button (using internal pullup resistor) is used
  to change state from INITIALIZE to MANUAL. The playstation circle 
  button is used to change the state from MANUAL to AUTONOMOUS.
*/
void updateStateMachine() {
  switch (RobotCurrentState) {
    case INITIALIZE:
      ps2x.read_gamepad();
      Serial.println("Initialize Case");
      if (ps2x.Button(PSB_SQUARE)) {
        RobotCurrentState = MANUAL;
      }
      break;

    case MANUAL:

      ps2x.read_gamepad();
      if (ps2x.ButtonPressed(PSB_CROSS) && !calibrated) {
        RobotCurrentState = CALIBRATE;
        calibrated = true;
      } else if (ps2x.ButtonPressed(PSB_CIRCLE) && calibrated) {
        RobotCurrentState = AUTONOMOUS;
        AutoCurrentState = START;
        break;
      } else
        break;

    case AUTONOMOUS: 
      Serial.println("Auton Case");
      if (!calibrated) { // Ensures the robot has been calibrated before line following
        RobotCurrentState = CALIBRATE;
        calibrated = true;
        break;
      }
      break;

    case CALIBRATE:
      Serial.println("Calibrate Case");
      break;
  }
}

/* executeStateActions function
  This function executes the proper actions for each state. 
  
  For an example, two functions are called, RemoteControl and 
  AutonomousControl. These functions are located in their respective
  files to make the code easier to read and modify.
*/
void executeStateActions() {
  switch (RobotCurrentState) {
    case INITIALIZE:
      // Perform actions for the initialize state
      Serial.println("Initializing...");
      // Add any additional actions for the initialize state
      break;

    case AUTONOMOUS:
      // Perform actions for the autonomous state
      Serial.println("Autonomous Mode");
      AutonomousControl();
      // Add any additional actions for the autonomous state
      break;

    case MANUAL:
      // Perform actions for the manual state
      RemoteControl();
      // Add any additional actions for the manual state
      break;

    case CALIBRATE:
      Serial.println("CALIBRATING");
      floorCalibration();
      RobotCurrentState = MANUAL;
      break;
  }
}
