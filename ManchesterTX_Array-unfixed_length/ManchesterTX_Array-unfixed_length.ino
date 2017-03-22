
#include <Manchester.h>

/*

  Manchester Transmitter example
  
  In this example transmitter will send 10 bytes array  per transmittion

  try different speeds using this constants, your maximum possible speed will 
  depend on various factors like transmitter type, distance, microcontroller speed, ...

  MAN_300 0
  MAN_600 1
  MAN_1200 2
  MAN_2400 3
  MAN_4800 4
  MAN_9600 5
  MAN_19200 6
  MAN_38400 7

*/

#define TX_PIN  3  //pin where your transmitter is connected
#define LED_PIN 13 //pin for blinking LED

uint8_t moo = 1; //last led status
uint8_t data[6] = {6, 1, 1, 1, 33, 5};

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, moo);
  man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_9600);
}


uint8_t datalength=6;   //at least two data
void loop() 
{

  data[5] = genChecksum(data[4], 8);
  man.transmitArray(datalength, data);

  delay(800);
  
}

//boolean checkChecksum(int data, int checksum, int numOfBits){
//  if (checksum == genChecksum(data, numOfBits) return true;
//  return false;
//}

int genChecksum(int data, int numOfBits){
  int checksum = 0;
  for(int i=0; i<numOfBits; i++){
    if(data & (int)pow(2, i)) checksum ++;
  }
  return checksum;
}

