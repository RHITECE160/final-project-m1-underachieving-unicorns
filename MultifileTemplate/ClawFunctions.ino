/* Claw Functions
Description: Code to handle opening and closing the claw
By: Connor Bodie 
Date: 1/22/24
*/

void openClaw(){ 
  myservo.write(140);
}
void closeClaw(){
  myservo.write(40);
}