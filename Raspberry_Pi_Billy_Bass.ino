/*
   Make two DC Motors Move to Sound.
   This example code is in the public domain.
   Created by Katrina Siegfried 2016, based on
   code by Donald Bell, Maker Project Lab (2016).
   Based on Sound to Servo by Cenk Özdemir (2012) 
   and DCMotorTest by Adafruit
*/

/* Things to note:
   1. You must wait at least 5 seconds before initiating Alexa commands at start
   2. Present audo gains, if cases, and interval used for 12V 1A power supply
*/


/* Things to add as of 11/29/2016:
   1. add filter to smooth out noise
   2. add billy bass speakers as audio output
   3. How does Pi output volume vs echo dot?
*/




// Include the Adafruit motor shield v2 library
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1 for mouth and M2 for tail
Adafruit_DCMotor *mouthMotor = AFMS.getMotor(1); // M1 is mouth motor, 64 kHz
Adafruit_DCMotor *headtailMotor = AFMS.getMotor(2); // M2 is head/tail motor, 64 kHz

// Define global variables
int SoundInPin = A0; // Input coming from Alexa audio in analog pin 0
unsigned long previousMillis = 0; // This is the starting time for the counter
const long interval = 3000; // This is the interval used to determine if the head motor should lift again



void setup() // the setup routine runs once when you press reset:
{
  
  Serial.begin(9600); // set up Serial library baud rate at 9600 bps
  
  AFMS.begin();  // create motor library with the default frequency 1.6KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed) for both motors
  mouthMotor->setSpeed(0); // mouth motor not moving
  mouthMotor->run(FORWARD); // turn on mouth motor
  mouthMotor->run(RELEASE); // release mouth motor
  headtailMotor->setSpeed(0); // tail motor not moving
  headtailMotor->run(FORWARD); // turn on mouth motor
  headtailMotor->run(RELEASE); // release mouth motor
  pinMode(SoundInPin, INPUT);  // sound from the alexa coming into the jack is INPUT
}




void loop() // the loop routine runs over and over again forever:
{

  
  uint8_t i; // Define i as an integer variable for counting mouth movement
  int sensorValue = analogRead(SoundInPin)*3; // Read the input on analog pin A0 and add a gain of 300%, this is for mouth motor, **GAIN MAY CHANGE WITH CURRENT INPUT**
  int MoveDelayValue = analogRead(SoundInPin)*3; //Read the input on analog pin A0 and add a gain of 300%, this is for head/tail motor, **GAIN MAY CHANGE WITH CURRENT INPUT**
  sensorValue = map(sensorValue,0,512,0,180); // Map it here down to the possible range of  movement, note normally the 512 is 1023 because of analog reading should go so far, but I changed that to get better readings. -Donald Bell
  MoveDelayValue = map(sensorValue,0,512,0,180); // Map it here down to the possible range of  movement, note normally the 512 is 1023 because of analog reading should go so far, but I changed that to get better readings. -Donald Bell 


  if (MoveDelayValue >= 9) // This is the threshold at which the head lifts, **THIS VALUE MAY CHANGE WITH CURRENT INPUT**
  { 
     unsigned long currentMillis = millis(); // This is the current time elapsed since start of Arduino board
     Serial.println(MoveDelayValue); // Print the MoveDelayValue to the serial monitor
     unsigned long motorSpeed = 230; // Assign the motorSpeed to 230, this is presently for the head/tail motor
     if (currentMillis - previousMillis < interval) // if the time elapsed since the head last moved, or was checked, is less than the interval value
     {
       headtailMotor->setSpeed(0); // Set the head/tail motor speed to 0
       headtailMotor->run(BRAKE); // Brake the head/tail motor (stop it in place with minimal resistance)
       Serial.println("Pause the head"); // Print "Pause the head" to the serial monitor
       Serial.println(MoveDelayValue); // Print the MoveDelayValue to the serial monitor 
       Serial.println(currentMillis); // Print the currentMillis to the serial monitor
       Serial.println(previousMillis); // Print the previous Millis to the serial monitor
       delay(1);  // Add a static delay to smooth things out
       previousMillis = currentMillis; // Update the previous time to the current time
     }
     else
     {
       Serial.println("Move the head");  // Print "Move the head" to the serial monitor
       //headtailMotor->run(RELEASE); // Release the head/tail motor
       delay(1); // Add a static delay to smooth things out
       headtailMotor->setSpeed(motorSpeed); // set the head/tail motor speed to the motorSpeed value
       headtailMotor->run(FORWARD); // move the head up (could be tail if you wired in opposite configuration)
       delay(150); // static delay so the head waits long enough
       headtailMotor->setSpeed(0); // turn off the head movement
       headtailMotor->run(BRAKE); // Brake, stop moving the head/tail motor
       Serial.println(MoveDelayValue); // Print the MoveDelay Value to the serial monitor
       Serial.println(currentMillis); // Print current time to the serial monitor
       Serial.println(previousMillis); // Print the previous time to the serial monitor
       delay(100); // static delay to smooth things out and create a short pause
       previousMillis = currentMillis; // Update the previous time to the current time
    }

    // Controlling mouth motor movement
    if (sensorValue > 11) // to cut off some static readings, otherwise the jaw chatters from noise **MAY CHANGE WITH CURRENT INPUT**
    { 
      mouthMotor->run(FORWARD); // now move the motor
      delay(1); // a static delay to smoothe things out
      for (i=150; i<255; i++) // control speed ramping, start at 150, count by 1, until 254 to speed up motor
      {
        mouthMotor->setSpeed(i);  // set the motor speed to whatever the value of the integer i is
      }    
      mouthMotor->run(RELEASE); // stop moving the mouth motor
      delay(1); // static delay to smooth things out
    }
 
   }
   else
   {
    //Serial.println(sensorValue); // Print the sensorValue to the serial monitor
    delay(1); // Add a static delay to smooth things out
    headtailMotor->setSpeed(0); // Set the head/tail motor to zero, stop it from moving forward
    headtailMotor->run(BRAKE); // Release the head/tail motor
    delay(1); // Add a static delay to smooth things out
    //Serial.println("Motor released "); // Print "Motor released" to the serial monitor
   }


// //LEAVE COMMENTED OUT FOR NOW, BUT PROBABLY DONT NEED THIS, CAN EVENTUALLY DELETE
//  // Controlling mouth motor movement
//  if (sensorValue > 11) // to cut off some static readings, otherwise the jaw chatters from noise
//  { 
//   delay(1);  // a static delay to smooth things out
//   mouthMotor->run(FORWARD); // now move the motor
//   delay(1); // a static delay to smoothe things out
//   for (i=110; i<255; i++) // control speed ramping, start at 140, count by 1, until 254 to speed up motor
//  {
//    mouthMotor->setSpeed(i);  // set the motor speed to whatever the value of the integer i is
//  }    
//  //Release control of both motors  
//  mouthMotor->run(RELEASE); // stop moving the mouth motor
//  //headtailMotor->run(RELEASE); // stop moving the head/tail motor
//  delay(1); // static delay to smooth things out
//  }


}


// Done

// Katrina Siegfried 2016
