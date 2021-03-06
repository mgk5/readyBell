
#include "Manchester.h" 

// include the library code:
#include <LiquidCrystal.h>

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

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define RX_PIN 6
#define LED_PIN 13

uint8_t moo = 1;
#define BUFFER_SIZE 18
uint8_t buffer[BUFFER_SIZE]; 


boolean checkChecksum(int data, int checksum, int numOfBits); 

int genChecksum(int data, int numOfBits); 

int bufferCount =0;

boolean fecResult;

void setup() 
{
  pinMode(LED_PIN, OUTPUT);  
  digitalWrite(LED_PIN, moo);
  Serial.begin(19200);
  man.setupReceive(RX_PIN, MAN_9600);
  man.beginReceiveArray(BUFFER_SIZE, buffer); 

    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.clear();
  lcd.print(""); 
   //Sound
  pinMode(7, OUTPUT);
  
}

void loop() 
{ 

    // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1); 
  
  if (man.receiveComplete()) 
  { 
    lcd.clear();
      digitalWrite(7,HIGH);
    delay(10);
    digitalWrite(7,LOW);
    
    uint8_t receivedSize = 16;      

    if(checkChecksum(buffer[4], buffer[5], 8))
    {
      Serial.println("Data is correct") ;
      lcd.print("Data Received");
    } 
    else 
    {
      Serial.println("ERROR");
    }
    for(uint8_t i=1; i<receivedSize; i++)
    {   
      Serial.print(String(buffer[i]) + " ");     
      man.beginReceiveArray(BUFFER_SIZE, buffer);
    } 
      Serial.println();
    //Forward Error Checking 
    fecResult = fecCheck(buffer);

     if(fecResult)
     {
      Serial.println("FEC Positive");
     }
     else
     {
      Serial.println("FEC Negative");
     }
   
    
    int tableNumber = buffer[4] >> 4;
    int count = buffer[4] & 0b00001111;
    Serial.println("table Number =" + String(tableNumber)+ " " + "Count Number= "+String(count));
    
 

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

boolean fecCheck(uint8_t* data){
    for(int i = 1; i < 6; i++){
        if(data[i] == data[i+5] && data[i] == data[i+10]){
            
        } else{
            return false;
        }
    }
    
    return true;

}
