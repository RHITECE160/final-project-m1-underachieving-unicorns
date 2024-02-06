
/*
 * Transmits an IR message given an address and a command
 * Uses TinyIRRemote Library
 * Author: John Webster
 */
 
#include <TinyIRremote.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define IR_TRX_PIN      18
#define IR_RCV_PIN      33

IRsender sendIR(IR_TRX_PIN);
IRreceiver irRX(IR_RCV_PIN);

IRData IRmsg;
IRData IRresults;

void LightCatrina(){
  Serial.println("Lighting Catrina");

  irRX.initIRReceiver();
  sendIR.initIRSender();

  if (irRX.decodeIR(&IRresults)) {
    Serial.print("A=0x");
    Serial.print(IRresults.address, HEX);
    Serial.print(" C=0x");
    Serial.print(IRresults.command, HEX);
    Serial.print(" is repeat: ");
    Serial.print(IRresults.isRepeat ? "TRUE" : "FALSE");
    Serial.println();

    IRmsg.isRepeat = false;
    IRmsg.protocol = NEC;
    IRmsg.address = 0xCE;
    IRmsg.command = IRresults.command;
    sendIR.write(&IRmsg);
  }
  else{
    Serial.println("No Data Received");
  }

  
  delay(500);
}

void LightVotive(){
  if (sendIR.initIRSender()) {
        Serial.println(F("Ready to transmit NEC IR signals on pin " STR(IR_TRX_PIN)));
    } else {
        Serial.println("Initialization of IR transmitter failed!");
        while (1) {;}
    }
  sendIR.initIRSender();

  IRmsg.isRepeat = false;
  IRmsg.protocol = NEC;
  IRmsg.address = 0xEE;
  IRmsg.command = 0xA0;
  sendIR.write(&IRmsg);
  Serial.println("Lighting Votive");

  delay(500);
}
