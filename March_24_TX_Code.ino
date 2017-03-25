
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

int genChecksum(int data, int numOfBits); 

int buttonFunction(int buttonRead);

uint8_t moo = 1; //last led status
uint8_t data[16] = {16, 1, 1, 1, 33, 5,  1, 1, 1, 33, 5,  1, 1, 1, 33, 5}; 
int count = 0;

const int buttonPin = 2;     // the number of the pushbutton pin 

const int ledIndicator = 4; 

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status 

// Variable for keeping the previous button state
// Variables will change
int ledState = HIGH;         // the current state of the output pin

int lastButtonState = LOW;   // the previous reading from the input pin
// the following variables are unsigned long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() 
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, moo);
  man.workAround1MhzTinyCore(); //add this in order for transmitter to work with 1Mhz Attiny85/84
  man.setupTransmit(TX_PIN, MAN_9600); 

   // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT); 
  pinMode(ledIndicator, OUTPUT);  

  // Define pin #2 as input and activate the internal pull-resistor
 pinMode(buttonPin, INPUT_PULLUP);
  // Establish the Serial connection with a baud rate of 115200
 Serial.begin(9600);
  
}


uint8_t datalength=16;   //at least two data

void loop() 
{
  int result = buttonFunction(digitalRead(buttonPin));    

 
}

//boolean checkChecksum(int data, int checksum, int numOfBits){
//  if (checksum == genChecksum(data, numOfBits) return true;
//  return false;
//} 

//Function to read buttonState 
int buttonFunction(int buttonRead)
{
  int reading = buttonRead;
  int prevLedState = ledState;
  
  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:    
    
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;      
      
      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {        
        ledState = !ledState;
      }
    }
  }

  // when led state has changed, send the data!
  if(prevLedState != ledState){
       count++;
       // set data as table1 with counter
       data[4] = 0b00010000 + count%4; 
       data[9] = data[4]; data[14] = data[4];
       
       // set checksum         
       data[5] = genChecksum(data[4], 8);
       data[10] = data[5]; data[15] = data[5];
       
       man.transmitArray(datalength, data);
       Serial.println("Data Sent");      
  }
  
  // set the LED:
  digitalWrite(ledIndicator, ledState);
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading; 
  return ledState;
}

int genChecksum(int data, int numOfBits){
  int checksum = 0;
  for(int i=0; i<numOfBits; i++){
    if(data & (int)pow(2, i)) checksum ++;
  }
  return checksum;
}
