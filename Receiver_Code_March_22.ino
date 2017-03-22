#include <Manchester.h>


int RX_PIN = 2;     // Receiver connected to digital pin 2

int val = 0;           // variable to store the value read 

char message[]="";

void setup()

{
  //Setup for receiving data
  man.setupReceive(RX_PIN, MAN_1200);
  man.beginReceive();
  Serial.begin(1200);          //  setup serial
  pinMode(RX_PIN, INPUT); 

}


void loop()

{ 

  // Loop for receiving data 

  if (man.receiveComplete()) {
    uint16_t m = man.getMessage();
    man.beginReceive(); 

    
   Serial.println(m); 
    
    //start listening for next message right after you retrieve the message
    //do something with your message here
  }
}
