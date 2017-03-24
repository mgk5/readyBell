
#include "Manchester.h"

/*

  Manchester Receiver example
  
  In this example receiver will receive array of 10 bytes per transmittion

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

#define RX_PIN 2
#define LED_PIN 13

uint8_t moo = 1;
#define BUFFER_SIZE 18
uint8_t buffer[BUFFER_SIZE]; 


boolean checkChecksum(int data, int checksum, int numOfBits); 

int genChecksum(int data, int numOfBits); 

int bufferCount =0;

void setup() 
{
  pinMode(LED_PIN, OUTPUT);  
  digitalWrite(LED_PIN, moo);
  Serial.begin(19200);
  man.setupReceive(RX_PIN, MAN_9600);
  man.beginReceiveArray(BUFFER_SIZE, buffer);
}

void loop() 
{
  if (man.receiveComplete()) 
  {
    uint8_t receivedSize = 6;      

    if(checkChecksum(buffer[4], buffer[5], 8))
    {
      Serial.println("Data is correct") ;
    } 
    else 
    {
      Serial.println("ERROR");
    }
    for(uint8_t i=1; i<receivedSize; i++)
    {   
      Serial.print(String(buffer[i]) + " ");     
          
    } 
   
    Serial.println();
    int tableNumber = buffer[4] >> 4;
    int count = buffer[4] & 0b00001111;
    Serial.println("table Number =" + String(tableNumber)+ " " + "Count Number= "+String(count));
  
    bufferCount++;
  
    man.beginReceiveArray(BUFFER_SIZE, buffer);
}  





}

int genChecksum(int data, int numOfBits){
  int checksum = 0;
  for(int i=0; i<numOfBits; i++){
    if(data & (int)pow(2, i)) checksum ++;
  }
  return checksum;
} 

boolean checkChecksum(int data, int checksum, int numOfBits){
  if (checksum == genChecksum(data, numOfBits)) return true; 
  return false;
}
