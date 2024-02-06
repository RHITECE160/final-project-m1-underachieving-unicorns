
/*
 * Transmits an IR message given an address and a command
 * Uses TinyIRRemote Library
 * Author: John Webster
 */
 
#include <TinyIRremote.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define IR_TRX_PIN      19

IRsender sendIR(IR_TRX_PIN);

IRData IRmsg;

void IRTransmit(int address, int command){
  IRmsg.protocol = NEC;
  IRmsg.address = address;
  IRmsg.command = command;
  IRmsg.isRepeat = false;
  sendIR.write(&IRmsg);
}
