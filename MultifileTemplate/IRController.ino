/*
 *  MinimalReceiver.ino
 *
 *  Small memory footprint and no timer usage!
 *
 *  Receives IR protocol data of NEC protocol using pin change interrupts.
 *  On complete received IR command the function decodeIR(IRData *results) may 
 *  be called to fetch results. Return value indicates if new data is available.
 *  Repeats are included by default, but may be disabled in call to init. 
 *  
 *  Multiple receiver objects may be specified with the Class IRreceiver.
 *  IR input pin must be specified in constructor.
 *
 *
 *  TinyIRremote is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as publi`ed by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */
#include <TinyIRremote.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define IR_RCV_PIN      33

IRreceiver irRX(IR_RCV_PIN);

IRData IRresults;
/* IR Controller - Code to handle IR Controller, maps button presses to robot movements. 
* (Modified example template)
* By: Connor Bodie, John Webster
* 1/23/24 
*/

void RemoteControlIR(){ //RemoteControlIR function, called in MultifileTemplate when we are demonstrating the IR controller 
  
  if (irRX.initIRReceiver()) {
        Serial.println(F("Ready to receive NEC IR signals at pin " STR(IR_RCV_PIN)));
    } else {
        Serial.println("Initialization of IR receiver failed!");
        while (1) {;}
  }

  while (CurrentRemoteMode == 1){  // setup from the example template 
    if (irRX.decodeIR(&IRresults)) {
        Serial.print("A=0x");
        Serial.print(IRresults.address, HEX);
        Serial.print(" C=0x");
        Serial.print(IRresults.command, HEX);
        Serial.print(" is repeat: ");
        Serial.print(IRresults.isRepeat ? "TRUE" : "FALSE");
        Serial.println();
    }
    int command = translateIR();
    Serial.println(command);
    switch(command){
      case 0x46: //"VOL+" Button for forward 
        forward();
        break;
      case 0x43: //"RIGHT" Button for turn right 
        turnRight();
        break;
      case 0x44:
        turnLeft(); //"LEFT" Button for turn left
        break;
      case 0x15: // "VOL-" Button for go backwards
        backward();
        break;
      case 0x9: // "UP" Button for open claw 
        openClaw();
        break;
      case 0x7: //"Down" Button for close claw
        closeClaw(); 
        break;
      default: // "Default" accessed by pressing any other button. Stops robot
        stop();
        break;
    }
  }
}

int translateIR() {                        // takes action based on IR code received
  int command = IRresults.command;
  delay(100);
  return command;
}



