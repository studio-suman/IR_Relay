/*
 * Works with all types of remotes (Configured for Panasonic and NEC types)
 * Uses button 2 & 3 for remotes to control relays
 * IR Receiver (TSOP2238) Pin1 - +5V, Pin2 - GND, Pin3 - Out/Signal
 * Output Pin - 12 & 13
 * Input Pin - 11
 * Pin Voltages - 5V
 * Clock - 16Mhz
 * Version 
 *  - 3.1(5th December, 2020) - Improved version dependency on Code from Stack exchange and Google
 *  - 3.2(24th April, 2020) - Improved version with added another NEC remote
 *  - Using v2.7 of IRremote Lib - https://github.com/Arduino-IRremote/Arduino-IRremote/commits/master/changelog.md
 *  Expected Enhancements - Addition of Programability new Remotes, EEPROM support
 * Coded by - Suman Saha
 */

#include <arduino.h>  
#include <IRremote.h>
 
  int RECV_PIN = 11; // the pin where you connect the output pin of TSOP4838
  int led1 = 12;
  int led2 = 13;
  int led3 = 10;
  int led4 = 9;
  int itsONled[] = {0,0,0,0,0};
  
  unsigned long CurrentValue = 0;          //value received from the infrared module
  unsigned long StoredCode = 0;            //value stored before

    /* the initial state of LEDs is OFF (zero) 
    the first zero must remain zero but you can 
    change the others to 1's if you want a certain
    led to light when the board is powered */
#define code1  0x1FED827  // code received from button 2
#define code2  0x1FEF807  // code received from button 3
#define code3  0x101C837  // code received from button 3
#define code4  0x101D827  // code received from button 3
#define code5  0x20240BF  // code received from button 2
#define code6  0x202C03F  // code received from button 3
#define codeW 0x4053888   // Panasonic button 2
#define codeV 0x4053848   //Panasonic button 3
//#define code3  [add your code] // code received from button C
//#define code4  [add your code] // code received from button D
  
IRrecv irrecv(RECV_PIN);

//IrReceiver;
 
decode_results results;
//IrReceiver.decodedIRData

 
void setup()
{
    Serial.begin(115200);   // you can comment this line
    irrecv.enableIRIn();  // Start the receiver
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__));

    // In case the interrupt driver crashes on setup, give a clue
    // to the user what's going on.
    Serial.println("Enabling IRin");
    irrecv.enableIRIn(); // Start the receiver

    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(RECV_PIN);
  
}
 
void loop() {

//  int value;
//  float volt;
//  value = analogRead(RECV_PIN);
//  volt = value * 5.0 / 1023.0;
//  Serial.print("Volt: ");
//  Serial.println(volt);
  if (irrecv.decode(&results)) {
  //Serial.println(results.decode_type);
      switch (results.decode_type) {
          case NEC:
            {
    //unsigned int value = results.value;
              switch(results.value) {
                 case code1:
                   if(itsONled[1] == 1) {        // if first led is on then
                      Serial.println("Turning on Relay 1: Value Received : ");
                      Serial.println(results.value,HEX);
                      digitalWrite(led1, LOW);   // turn it off when button is pressed
                      itsONled[1] = 0;           // and set its state as off
                   } else {                      // else if first led is off
                       digitalWrite(led1, HIGH); // turn it on when the button is pressed
                       itsONled[1] = 1;          // and set its state as on
                   }
                    break; 
                 case code2:
                   if(itsONled[2] == 1) {
                      Serial.println("Turning on Relay 2: Value Received : ");
                      Serial.println(results.value,HEX);
                      digitalWrite(led2, LOW);
                      itsONled[2] = 0;
                   } else {
                       digitalWrite(led2, HIGH);
                       itsONled[2] = 1;
                   }
                    break;       
                case code3:
                   if(itsONled[1] == 1) {
                    Serial.println("Turning on Relay 1(NEC2): Value Received : ");
                      digitalWrite(led1, LOW);
                      itsONled[1] = 0;
                   } else {
                       digitalWrite(led1, HIGH);
                       itsONled[1] = 1;
                   }
                    break; 
                case code4:
                   if(itsONled[2] == 1) {
                    Serial.println("Turning on Relay 2(NEC2): Value Received : ");
                      digitalWrite(led2, LOW);
                      itsONled[2] = 0;
                   } else {
                       digitalWrite(led2, HIGH);
                       itsONled[2] = 1;
                   }
                    break;   
                case code5:
                   if(itsONled[1] == 1) {
                    Serial.println("Turning on Relay 1(NEC3): Value Received : ");
                      digitalWrite(led1, LOW);
                      itsONled[1] = 0;
                   } else {
                       digitalWrite(led1, HIGH);
                       itsONled[1] = 1;
                   }
                    break;  
               case code6:
                   if(itsONled[2] == 1) {
                    Serial.println("Turning on Relay 2(NEC3): Value Received : ");
                      digitalWrite(led2, LOW);
                      itsONled[2] = 0;
                   } else {
                       digitalWrite(led2, HIGH);
                       itsONled[2] = 1;
                   }
                    break;      
                }
        break;
        }
      case PANASONIC:
        {
          switch(results.value) {
                 case codeV:
                   if(itsONled[1] == 1) {        // if first led is on then
                      Serial.println("Turning on Relay 1: Value Received : ");
                      Serial.println(results.value,HEX);
                      digitalWrite(led1, LOW);   // turn it off when button is pressed
                      itsONled[1] = 0;           // and set its state as off
                   } else {                      // else if first led is off
                       digitalWrite(led1, HIGH); // turn it on when the button is pressed
                       itsONled[1] = 1;          // and set its state as on
                   }
                    break; 
                 case codeW:
                   if(itsONled[2] == 1) {
                      Serial.println("Turning on Relay 2: Value Received : ");
                      Serial.println(results.value,HEX);
                      digitalWrite(led2, LOW);
                      itsONled[2] = 0;
                   } else {
                       digitalWrite(led2, HIGH);
                       itsONled[2] = 1;
                   }
        }
    break;
      } 
    }
  irrecv.resume();  
  delay(100);
  }
}  
void blnk() 

{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                  
}
